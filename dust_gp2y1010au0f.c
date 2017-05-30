
// The SHARP Dust sensor is triggered by sending a low (0v) pulse
// - The period of this pulse is 10ms 
// - The pulse width is 0.32 ms
// - The analog readout is ready 0.28 ms after the low-edge of the pulse

int16 DUST_GP2Y_update() {
   int16 dust_level_ref, dust_level, diff;
   int readCount=0, maxRead=5;
   
   output_high(DUST_GP2Y_IO);  // turn off the sensor LED
   
   setup_adc(ADC_CLOCK_INTERNAL);
   setup_adc_ports(sAN2);
   set_adc_channel(2);
   delay_ms(1);
   
   dust_level_ref = DUST_GP2Y_read();
   
   do {
      dust_level = DUST_GP2Y_read();
      diff = abs(dust_level - dust_level_ref);
      dust_level_ref = dust_level;
   
   } while ((diff > DUST_GP2Y_THRESHOLD) && (++readCount < maxRead));
   
 
   return(dust_level);
   
}

int16 DUST_GP2Y_read() {
   int16 dust_level;
   
   output_low(DUST_GP2Y_IO);
   delay_us(280);
   read_adc(ADC_START_ONLY );
   delay_us(40);
   output_high(DUST_GP2Y_IO);
   dust_level = read_adc(ADC_READ_ONLY);
   delay_us(9680);

   return(dust_level);

}
