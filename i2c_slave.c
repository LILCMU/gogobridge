
#INT_SSP
void ssp_interrupt()
{
   unsigned int8 incoming, state;

   state = i2c_isr_state();

   if(state <= 0x80)                      //Master is sending data
   {
      if(state == 0x80)
         incoming = i2c_read(2);          //Passing 2 as parameter, causes the function to read the SSPBUF without releasing the clock
      else
         incoming = i2c_read();

      if(state == 1)                      //First received byte is address
         gblRegisterAddress = incoming;
      else if(state >= 2 && state != 0x80) {  //Received byte is data
         deviceRegister[gblRegisterAddress] = incoming;
         if (gblRegisterAddress == DEVICE_DEF) {
            gblUpdateDeviceValue = 1;
         }
         gblRegisterAddress++;
      }
   }

   if(state >= 0x80)                      //Master is requesting data
   {
      i2c_write(deviceRegister[gblRegisterAddress++]);
   }

   

   
}


//!void main() {
//!
//!     int i;
//!
//!     enable_interrupts(INT_SSP);
//!     enable_interrupts(GLOBAL);
//!     
//!     while (1) {
//!           printf("Press any key to view slave registers:");
//!           getc();
//!           
//!           printf("\r\nDevice Register = [ ");
//!           for (i=0;i<REGISTER_SIZE;i++) {
//!               printf("%u ",deviceRegister[i] ); 
//!           }
//!           printf("]\r\n");
//!     }
//!
//!}
