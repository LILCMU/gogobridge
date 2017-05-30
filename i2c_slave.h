
///////////////////////////////////////////////////////////////////////////
// Project constants
///////////////////////////////////////////////////////////////////////////
#define SLAVE_ADDRESS  0xC0     // the I2C address. Must be an even number
#define REGISTER_SIZE  64       // the slave's register size. Resize to 
                                // fit your needs


// setup the I2C port
#PIN_SELECT SCL1IN=PIN_C0
#PIN_SELECT SCL1OUT=PIN_C0
#PIN_SELECT SDA1IN=PIN_C1
#PIN_SELECT SDA1OUT=PIN_C1
#use i2c(SLAVE, I2C1, address=SLAVE_ADDRESS, FORCE_HW)



///////////////////////////////////////////////////////////////////////////
// This is the main device register
///////////////////////////////////////////////////////////////////////////
int deviceRegister[REGISTER_SIZE]={0}; // device register
int gblRegisterAddress=0;              // register index


#INT_SSP
void ssp_interrupt();
