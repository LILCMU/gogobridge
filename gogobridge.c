//
// GoGo Bridge - firmware for the GoGo Bridge sensor interface board
//
// Copyright (C) 2017 Chiang Mai University
// Contact   Arnan (Roger) Sipiatkiat [arnans@eng.cmu.ac.th]
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation version 3.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//



#include <16F1705.h>
#device adc=10
#case


#FUSES INTRC_IO      //Internal RC Osc, on CLKOUT
#FUSES NOWDT         //No Watch Dog Timer
//#FUSES HS            //High speed Osc (> 4mhz for PCM/PCH) (>10mhz for PCD)
#FUSES NOMCLR //Master Clear pin disabled
#FUSES BROWNOUT // brownout reset
#FUSES BORV25
#FUSES LVP           //Low Voltage Programing
#FUSES PUT                      //Power Up Timer
//#FUSES NOCPD                    //No EE protection
#FUSES IESO                     //Internal External Switch Over mode enabled
#FUSES FCMEN                    //Fail-safe clock monitor enabled
#FUSES NODEBUG                  //No Debug mode for ICD
#FUSES NOWRT                    //Program memory not write protected
#FUSES RESERVED                 //Used to set the reserved FUSE bits
#FUSES NOPPS1WAY                // Allow multiple configurations of peripheral pins


#define GOGO_BRIDGE_VERSION      5

#use delay(clock=32 MHz)
#use rs232(baud=115200,xmit=PIN_C4,rcv=PIN_C5)

int1 gblUpdateDeviceValue=0;


// ============================================================================
// Register map (64 bytes)
//
// The gogo bridge emulates an EEPROM read/write process for simpilcity. Each memory 
// location are allocated as follows. 
// 
// - Address 0-4 are reserved for configuration purposes. The current locations in
//   use are
//    0 - firmware version
//    1 - target device selection (see Device Definition below). 
//
// - Address 5-63 are used to store sensor values
// ============================================================================
#define FIRMWARE_VERSION         0     
#define DEVICE_DEF               1     // device definition. tells us what sensor is used
#define CURRENT_LOW              5     // Current transformer readings
#define CURRENT_HIGH             6
#define DUST_GP2Y_LOW            7     // DUST level from the SHARP GP2Y
#define DUST_GP2Y_HIGH           8
#define DHT_TEMPERATURE          9     // temperature from DHT11, DHT22
#define DHT_TEMP_DECIMAL         10    // decimal temperature from DHT22
#define DHT_RH                   11    // RH from DHT11, DHT22
#define DHT_RH_DECIMAL           12    // decimal RH from DHT22
#define ULTRASONIC_LOW           13    // ultrasonic range low byte (unit in cm)
#define ULTRASONIC_HIGH          14    // ultrasonic range high byte
#define ULTRASONIC2_LOW          15    // ultrasonic 2 range low byte (unit in cm)
#define ULTRASONIC2_HIGH         16    // ultrasonic range high byte

#define WATER_FLOW_LOW           17    // Water flow sensor
#define WATER_FLOW_HIGH          18    
#define WATER_FLOW2_LOW          19    // Water flow sensor 2
#define WATER_FLOW2_HIGH         20    
// We support up to 8 daisy chained DS18B20 temperature sensors
#define DS18B20_Count            21    // The number of sensors found
#define DS18B20_1                22    // 1st DS18B20 temperature
#define DS18B20_1_DECIMAL        23    // 1st DS18B20 temperature decimal
#define DS18B20_2                24    // ...
#define DS18B20_2_DECIMAL        25
#define DS18B20_3                26
#define DS18B20_3_DECIMAL        27
#define DS18B20_4                28
#define DS18B20_4_DECIMAL        29
#define DS18B20_5                30
#define DS18B20_5_DECIMAL        31
#define DS18B20_6                32
#define DS18B20_6_DECIMAL        33
#define DS18B20_7                34
#define DS18B20_7_DECIMAL        35
#define DS18B20_8                36
#define DS18B20_8_DECIMAL        37
// second DHT sensor
#define DHT2_TEMPERATURE         38  // decimal temperature from the second DHT11, DHT22
#define DHT2_TEMP_DECIMAL        39    // decimal temperature from DHT22
#define DHT2_RH                  40    // RH from DHT11, DHT22
#define DHT2_RH_DECIMAL          41    // decimal RH from DHT22
#define HARTRATE                 42    // hear rate pulse sensor


// ===========================================================================
// Device definition
//
// Valuese for deviceRegister[DEVICE_DEF].
// ===========================================================================

#define DHT11     11
#define DHT22     22
#define DHT11_2   112
#define DHT22_2   222
#define HC_SR04   04       // ultrasonic sensor
#define HC_SR04_2 05       // ultrasonic sensor 2
#define YF_S201   12       // water flow sensor YF-S201 1-30L/min
#define YF_S201_2 13       // water flow sensor YF-S201 second unit
#define DS18B20   14
#define DUST_GP2Y 15       // SHARP dust densor gp2y1010au0f
#define CURRENT   16       // A current transformer or current sensor input (assumes an AC signal)

#define PIN_HEARTBEAT   PIN_A5   // heart beat LED

// Header files for the code implementing each device
#include "DHT.h"
#include "i2c_slave.h"
#include "ultrasonic.h"
//#include "encoder.h"
#include "waterflow.h"
#include "ds18b20.h"
#include "dust_gp2y1010au0f.h"
#include "current.h"
#include "heartrate.h"

int targetDevice;

// Registers used to check and clear I2C errors
#bit SSPEN = 0x14.5
#bit SSPOV = 0x14.6
#bit WCOL  = 0x14.7

void resetI2C() {
   SSPEN = 0;   // disable i2c
   SSPOV = 0;   // clear the receive overflow flag
   WCOL = 0;    // clear the write collision flag

   SSPEN = 1;   // re-enable i2c

}




void main()
{

   int16 range;
   int16 counter;
   int16 heartBeatCounter=0;
   int16 foo;
   int i=0;

   enable_interrupts(INT_SSP);
   enable_interrupts(GLOBAL);
  
   deviceRegister[0] = GOGO_BRIDGE_VERSION;   // the firmware version ID
  
//  deviceRegister[DEVICE_DEF] = CURRENT;   // fix to a curent sensor for testing.
  
  
   while(1)
   {
   
      delay_ms(1);
      if (heartBeatCounter++ == 500) {
         output_toggle(PIN_HEARTBEAT);
         heartBeatCounter = 0;
//         gblUpdateDeviceValue=1;  // trigger a sensor reading for testing
      }
   
      // wait until the I2C master (the GoGo Board) assigns a terget device
      // This is a simple synchronization method that prevents the 
      // device from communicating with a sensor during an i2c comm event 
      // The master needs to explicitely give time for this device
      // to communicate with the target sensor after assigning a target device.
      if (!gblUpdateDeviceValue) {
         continue;
      }
      
      gblUpdateDeviceValue = 0;
      output_toggle(PIN_HEARTBEAT);
      
      targetDevice = deviceRegister[DEVICE_DEF];
      
      if ((targetDevice == DHT11) || (targetDevice == DHT22)) {
         if (targetDevice == DHT11) { DHT_MODEL = DHT11;} 
         else { DHT_MODEL = DHT22; };

         disable_interrupts(INT_SSP);
         DHT_update(1);
         enable_interrupts(INT_SSP);
         
         if (targetDevice == DHT11) {
            deviceRegister[DHT_TEMPERATURE] = DHT_temperature;
            deviceRegister[DHT_TEMP_DECIMAL] = 0;
            deviceRegister[DHT_RH] = DHT_humidity;
            deviceRegister[DHT_RH_DECIMAL] = 0;
         }   
         if (targetDevice == DHT22) {
            deviceRegister[DHT_TEMPERATURE] = DHT_temperature >> 8;     
            deviceRegister[DHT_TEMP_DECIMAL] = DHT_temperature & 0xff;  // decimal value
            deviceRegister[DHT_RH] = DHT_humidity >> 8;
            deviceRegister[DHT_RH_DECIMAL] = DHT_humidity & 0xff;    // decimal value      
         
         }
      }
      
      // ===============  Second DHT
      if ((targetDevice == DHT11_2) || (targetDevice == DHT22_2)) {
         if (targetDevice == DHT11_2) { DHT_MODEL = DHT11;} 
         else { DHT_MODEL = DHT22; };

         disable_interrupts(INT_SSP);
         DHT_update(2);
         enable_interrupts(INT_SSP);
         
         if (targetDevice == DHT11_2) {
            deviceRegister[DHT2_TEMPERATURE] = DHT_temperature;
            deviceRegister[DHT2_TEMP_DECIMAL] = 0;
            deviceRegister[DHT2_RH] = DHT_humidity;
            deviceRegister[DHT2_RH_DECIMAL] = 0;
         }   
         if (targetDevice == DHT22_2) {
            deviceRegister[DHT2_TEMPERATURE] = DHT_temperature >> 8;     
            deviceRegister[DHT2_TEMP_DECIMAL] = DHT_temperature & 0xff;  // decimal value
            deviceRegister[DHT2_RH] = DHT_humidity >> 8;
            deviceRegister[DHT2_RH_DECIMAL] = DHT_humidity & 0xff;    // decimal value      
         
         }
      }      
      
      
      // ultrasonic range finder
      if (targetDevice == HC_SR04) {
         ULTRASONIC_MODEL=HC_SR04;
         disable_interrupts(INT_SSP);
         range = ULTRASONIC_update();  
         enable_interrupts(INT_SSP);
         
         deviceRegister[ULTRASONIC_LOW] = range & 0xff;
         deviceRegister[ULTRASONIC_HIGH] = range >> 8;

         
      }
   
      // 2nd ultrasonic sensor  
     if (targetDevice == HC_SR04_2) {
         ULTRASONIC_MODEL=HC_SR04;
         disable_interrupts(INT_SSP);
         range = ULTRASONIC2_update();  
         enable_interrupts(INT_SSP);
         
         deviceRegister[ULTRASONIC2_LOW] = range & 0xff;
         deviceRegister[ULTRASONIC2_HIGH] = range >> 8;
         
      }
   
      // water flow sensor
      if (targetDevice == YF_S201) {
         setup_waterflow();
         WATER_FLOW_MODEL = YF_S201;
         counter = update_waterflow();
         deviceRegister[WATER_FLOW_LOW] = counter & 0xff;
         deviceRegister[WATER_FLOW_HIGH] = counter >> 8;
      
      }
         
      if (targetDevice == YF_S201_2) {
         setup_waterflow2();
         WATER_FLOW_MODEL = YF_S201;
         counter = update_waterflow2();
         deviceRegister[WATER_FLOW2_LOW] = counter & 0xff;
         deviceRegister[WATER_FLOW2_HIGH] = counter >> 8;
      
      }   
      
      if (targetDevice == DS18B20) {
            init_DS18B20();   // scans for temperature sensors on the 1-wire bus
         
            update_DS18B20();
      }

         
      if (targetDevice == DUST_GP2Y) {
         foo = DUST_GP2Y_update();
         deviceRegister[DUST_GP2Y_LOW] = foo & 0xff;
         deviceRegister[DUST_GP2Y_HIGH] = foo >> 8;
      
      }
      
      if (targetDevice == CURRENT) {
         foo = calcIrms(200);
         deviceRegister[CURRENT_LOW] = foo & 0xff;
         deviceRegister[CURRENT_HIGH] = foo >> 8;         
      
      }
         
      output_toggle(PIN_HEARTBEAT);
     
   
   }
}





#include "i2c_slave.c"
#include "DHT.c"
#include "ultrasonic.c"
//#include "encoder.c"
#include "waterflow.c"
#include "ds18b20.c"
#include "dust_gp2y1010au0f.c"
#include "current.c"
#include "heartrate.c"
