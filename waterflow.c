void setup_waterflow() {
   if (!waterFlowAlreadyInitialized) {

   setup_ccp1(CCP_CAPTURE_RE);    // Configure CCP1 to capture rise
   enable_interrupts(INT_CCP1);   // Setup interrupt on falling edge
   enable_interrupts(GLOBAL);
   
   }
}

void reset_waterflow() {
      waterflow_counter=0;
      setup_ccp1(CCP_CAPTURE_RE);    // Configure CCP1 to capture rise
      enable_interrupts(INT_CCP1);   // Setup interrupt on falling edge
      enable_interrupts(GLOBAL);
         
}


#int_ccp1
void ccp1_waterflow_isr() {
   waterflow_counter++;   
}


int16 update_waterflow() {

   if (WATER_FLOW_MODEL == YF_S201) {
      return waterflow_counter;
   }
}

// =======================================
// Second water flow
// =======================================


void setup_waterflow2() {
   if (!waterFlow2AlreadyInitialized) {

   setup_ccp2(CCP_CAPTURE_RE);    // Configure CCP1 to capture rise
   enable_interrupts(INT_CCP2);   // Setup interrupt on falling edge
   enable_interrupts(GLOBAL);
   
   }
}

void reset_waterflow2() {
      waterflow2_counter=0;
      setup_ccp2(CCP_CAPTURE_RE);    // Configure CCP1 to capture rise
      enable_interrupts(INT_CCP2);   // Setup interrupt on falling edge
      enable_interrupts(GLOBAL);
         
}


#int_ccp2
void ccp2_waterflow_isr() {
   waterflow2_counter++;   
}

int16 update_waterflow2() {

   if (WATER_FLOW_MODEL == YF_S201) {
      return waterflow2_counter;
   }



}



