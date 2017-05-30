float offsetI=512; 

int16 calcIrms(int16 Number_of_Samples)
{  
int16 sampleI0;
float filteredI0;
 
float sqI0;
float sumI0=0;
float Irms0=0;
int16 n;
   
    setup_dac (DAC_VSS_VDD | DAC_OUTPUT2);  // output V to PIN_A2 (PORT P1)
    dac_write(128);  // should produce VDD/2 or 1.65 Vot

    setup_adc(  ADC_CLOCK_INTERNAL );
    setup_adc_ports(CT_INPUT_CH);
    setup_adc_reference(VSS_VDD);
    set_adc_channel(CT_INPUT_AN_PORT);
    //set_adc_channel(DAC_CHANNEL);
    delay_ms(1);
   
   for (n = 0; n < Number_of_Samples; n++) {
      sampleI0 = read_adc();
   
      offsetI = offsetI + (float)(sampleI0-offsetI)/1024.0; // weighted offset adjustment
      filteredI0 = sampleI0 - offsetI;  // apply the offset
             
      sqI0 = filteredI0 * filteredI0;  
      sumI0 += sqI0;

      
      delay_us(500);
   }  
   Irms0 =  sqrt(sumI0 / Number_of_Samples);
   
   
   
   //if(Irms0<5){Irms0=0;} 

  
//   sampleI0 = read_adc();
   //printf("adc=%lu\r\n", sampleI0);
//!   printf("Sum = %f\r\n", offsetI);
//!   printf("I = %f\r\n", Irms0);
   
   return((int16)Irms0);
}






//!int16 calcIrms(int16 Number_of_Samples) {
//!
//!
//!float SupplyVoltage = 5000.0;
//!//int16 ADC_COUNTS = 1 << ADC_BITS;
//!int16 ADC_COUNTS = 0;
//!int16 sampleI0;
//!int16 sampleI1;
//!
//!float filteredI0;
//!float filteredI1;
//!
//!int16 offsetI=0; //Low-pass filter output
//!float sqI0;
//!float sqI1;
//!float sumI0;
//!float sumI1;
//!//Set Voltage and current input pins
//!//Calibration coefficients
//!//These need to be set in order to obtain accurate results
//!float Irms0;
//!float Irms1;
//!float temp0,temp01;
//!float temp1,temp11;
//!int1 calIsRunning=0;
//!int holdDeviceRegister[4]={0};
//!
//!float ICAL=6.0;
//!int iic=0;
//!int16 value0;
//!int16 value1;
//!
//!
//!
//!   calIsRunning=1;
//!   Irms0=0;
//!   Irms1=0;
//!   //printf("I= start %d mA\r\n",iic);
//!   float offsetI_temp0=offsetI;
//!   float offsetI_temp1=offsetI;
//!   for (int16 n = 0; n < Number_of_Samples; n++)
//!   {
//!
//!
//!      sampleI0 = read_adc();
//!   
//!
//!   
//!      offsetI_temp0 = (offsetI_temp0 + ((sampleI0-offsetI_temp0)/1024)); 
//!      filteredI0 = sampleI0 - offsetI_temp0;
//!
//!      
//!      sqI0 = filteredI0 * filteredI0;
//!      sumI0 += sqI0;
//!
//!      delay_us(500);
//!   }  
//!
//!   Irms0 =  sqrt(sumI0 / Number_of_Samples);
//!
//!   if(Irms0<5){Irms0=0;}
//!
//!   
//!
//!   
//!   temp0 = (((Irms0/1024)*5000)/185);
//!   temp01= temp0-(temp0*0.04);
//!   if(temp0<0.15){temp01=temp01-0.02;}
//!   if(temp01<0){temp01=0;}
//!  
//!   printf("I= %f A\r\n",Irms0);
//!   
//!   
//!
//!
//!  //Reset accumulators
//!   sumI0 = 0;
//!   sumI1 = 0;
//!   calIsRunning=0;
//!
//!  
//!  return((int16)Irms0);
//!  
//!}




