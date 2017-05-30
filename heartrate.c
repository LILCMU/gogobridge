

void calibrateHR()  {  


}

void setupHR() {

   int threshold;
   int i;

   
   setup_dac (DAC_VSS_VDD | DAC_OUTPUT);
   setup_adc_ports( NO_ANALOGS );
   setup_adc(ADC_CLOCK_INTERNAL );
   set_adc_channel(DAC_CHANNEL);

     
   for (i=0;i<256;i++) {
      //output_toggle(PIN_HEARTBEAT);
      dac_write(i);   
      delay_ms(100);
      printf("%u - %lu\r\n", i, read_adc());
      
   }





}


