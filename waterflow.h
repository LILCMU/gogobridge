int WATER_FLOW_MODEL  = YF_S201;

int1 waterFlowAlreadyInitialized=0;
int16 waterflow_counter=0;

void setup_waterflow();
void reset_waterflow();
int16 update_waterflow();
#int_ccp1
void ccp1_waterflow_isr();


// Second flow sensor
int1 waterFlow2AlreadyInitialized=0;
int16 waterflow2_counter=0;

void setup_waterflow2();
void reset_waterflow2();
int16 update_waterflow2();
#int_ccp2
void ccp2_waterflow_isr2();
