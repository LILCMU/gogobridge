void setup_encoder() {
   setup_ccp1(CCP_CAPTURE_RE);    // Configure CCP1 to capture rise
   setup_ccp2(CCP_CAPTURE_FE);    // Configure CCP2 to capture fall

   enable_interrupts(INT_CCP1);   // Setup interrupt on falling edge
   enable_interrupts(INT_CCP2);   // Setup interrupt on falling edge
   enable_interrupts(GLOBAL);   



}

#int_ccp1
void ccp1_encoder_isr() {
   if (input(CCP1_PIN) && input(CCP2_PIN)) {
      encoder_counter--;
      setup_ccp1(CCP_CAPTURE_FE);
   } 
   if (input(CCP1_PIN) && !input(CCP2_PIN)) {
      encoder_counter++;
      setup_ccp1(CCP_CAPTURE_FE);
   }
   
   if (!input(CCP1_PIN) && input(CCP2_PIN)) {
      encoder_counter++;
      setup_ccp1(CCP_CAPTURE_RE);
   } 
   if (!input(CCP1_PIN) && !input(CCP2_PIN)) {
      encoder_counter--;
      setup_ccp1(CCP_CAPTURE_RE);
   }

}

#int_ccp2
void ccp2_encoder_isr() {
   if (input(CCP1_PIN) && input(CCP2_PIN)) {
      encoder_counter++;
      setup_ccp2(CCP_CAPTURE_FE);
   } 
   if (input(CCP1_PIN) && !input(CCP2_PIN)) {
      encoder_counter--;
      setup_ccp2(CCP_CAPTURE_RE);
   }
   
   if (!input(CCP1_PIN) && input(CCP2_PIN)) {
      encoder_counter--;
      setup_ccp2(CCP_CAPTURE_FE);
   } 
   if (!input(CCP1_PIN) && !input(CCP2_PIN)) {
      encoder_counter++;
      setup_ccp2(CCP_CAPTURE_RE);
   }

}

int16 read_encoder()
{
   return(encoder_counter);
}
