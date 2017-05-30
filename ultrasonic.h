
#define ULTRASONIC_TRIG    PIN_A1
#define ULTRASONIC_ECHO    PIN_A0


int ULTRASONIC_MODEL=HC_SR04;

int16 ULTRASONIC_update();
int16 timeCounter=0;


#define ULTRASONIC2_TRIG    PIN_A5
#define ULTRASONIC2_ECHO    PIN_A4


int16 ULTRASONIC2_update();
int16 timeCounter2=0;

