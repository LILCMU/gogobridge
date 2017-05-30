int16 ULTRASONIC_update() {

   output_high(ULTRASONIC_TRIG);                         // ping the sonar 
   delay_us(20);                            // sending 20us pulse 
   output_low(ULTRASONIC_TRIG); 
   timeCounter=0;
   
   while(!input(ULTRASONIC_ECHO))   {                    // wait for high state of echo pin 
      delay_us(1);
   }
   
  
   while(input(ULTRASONIC_ECHO)) {                      // Wait for high state of echo pin 
     timeCounter++;
     delay_us(10);
   }
   
   //return(timeCounter);
   return((timeCounter*10/48) + 17);


   
}


// ============================
// second ultrasonic sensor
// ============================


int16 ULTRASONIC2_update() {

   output_high(ULTRASONIC2_TRIG);                         // ping the sonar 
   delay_us(20);                            // sending 20us pulse 
   output_low(ULTRASONIC2_TRIG); 
   timeCounter2=0;
   
   while(!input(ULTRASONIC2_ECHO))   {                    // wait for high state of echo pin 
      delay_us(1);
   }
   
  
   while(input(ULTRASONIC2_ECHO)) {                      // Wait for high state of echo pin 
     timeCounter++;
     delay_us(10);
   }
   
   //return(timeCounter);
   return((timeCounter2*10/48) + 17);


   
}

