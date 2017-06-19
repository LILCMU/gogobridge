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


#use delay(clock=32 MHz)
#use rs232(baud=115200,xmit=PIN_C4,rcv=PIN_C5)


#define  bool   int1
#define  byte   int

#define  LOW   0
#define  HIGH  1
