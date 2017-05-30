// ==============================
// Select the hardware being used
// ==============================
//#define GOGOBRIDGE
#define GEKKO


#INCLUDE <math.h>








#ifdef GEKKO

   // PIN OUT. The two terminals of the CT connect to:
   // PIN_C2 - Analog input (AN6)
   // PIN_A2 - DAC reference voltage (VDD/2)
  
   #define CT_INPUT_CH              sAN6   // analog channel constant
   #define CT_INPUT_AN_PORT         6      // analog channel

#endif

#ifdef GOGOBRIDGE
   // PIN OUT. The two terminals of the CT connect to:
   // PIN_C3 - Analog input (AN7)
   // PIN_A2 - DAC reference voltage (VDD/2)
   
   
   #define CT_INPUT_CH              sAN7   // analog channel constant
   #define CT_INPUT_AN_PORT         7      // analog channel

#endif

int16 calcIrms(int16 Number_of_Samples);

float filteredI0=0;





