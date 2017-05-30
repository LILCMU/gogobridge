# gogobridge
Firmware for the GoGo Bridge sensor interface board.

# Introduction
The GoGo Bridge is a small interface board that allows the GoGo Board to interface with various sensors via an I2C port. It is driven by a PIC16F1705 mcu. The source is written with the CCS PIC-C compiler. 

gogobridge.c is the main source file.

Currently the GoGo Bridge supports the following sensors:
- DHT11 and DHT22 Temperature and Relative Humidity sensors
- HC_SR04 ultrasonic range sensor
- YF_S201 water flow sensor
- DS18B20 temperature sensor
- DUST_GP2Y SHARP dust sensor
- Current transformers. Any CT that outputs a 0-5V range.

# I2C Communication

The default I2C 8 bit address is 0xC0. The GoGo Bridge imulates an EEPROM for the read/write operations. A device register provides a 
buffer for all the supported sensor readings. See gogobridge.c for a full definition of this register. 

Here is a brief description of how to read a value from the GoGo Bridge
1. The GoGo Board (or any I2C master device) must tell the GoGo Bridge what sensor to use. This is done by writing a device ID to
   address 1 of the device register. See gogobridge.c for a full list of device IDs.
2. Once a device ID is received, the GoGo Bridge reads from that device and stores the resulting value in the corresponding 
   buffer location. Note that the GoGo Board must wait for the GoGo Bridge to complete this operation. There is currently no way to
   know when the GoGo Bridge is done with the sensor reading. A delay of 100 ms is usually more than enough.
3. The GoGo Board reads the result from the corresponding register address.

Note. The step 1 above is required each time the GoGo Board wants to read a sensor value. It is designed as a simple 
      synchronization method. In early versions, we allowed the GoGo Bridge to continuously monitor the sensor. 
      This caused a problem when an i2c activity occurs while the GoGo Bridge is busy talking to a sensor. 

# Adding Support for a New Sensor

In the file gogobridge.c:
1. Allocate locations in the deviceRegister array. 
   This array is currently 64 bytes and any free locations can be used. 
2. Define a device ID (definition) for the new sensor.
3. Append a condition that checks for the device ID from the GoGo Board. You should call the specific functions that
   reads from the sensor here.
4. Add an #include to the header and C source file for you sensor

The steps are quite simple. Look at how other sensors are implemented to learn more.

