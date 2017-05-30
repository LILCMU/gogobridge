
/////////////////////////////////////////////////////////////////////////////////////////////////////
//                   Sensor Temperature & Humidity Function                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////


void DHT_Start(void) 
{ 
   // Here we send the 'start' sequence to the DHT sensor! We pull the DHT_IO pin low for 25mS, and 
   // then high for 30mS. The DHT sensor then responds by pulling the DHT_IO pin low, and then pulling it 
   // back high. 
    
   output_low(DHT_IO); 
   delay_ms(25); 
   output_high(DHT_IO); 
   delay_us(30); 
    
   // Here we wait while the DHT_IO pin remains low..... 
   bExit = False; 
   set_timer0(0); 
   while (!input(DHT_IO) && !bExit); 

   // Here we wait while the DHT_IO pin remains high..... 
   bExit = False; 
   set_timer0(0); 
   while (input(DHT_IO) && !bExit); 
} 

int8 DHT_ReadData(void) 
{ 
    
   // This subroutine reads a byte of data (8 bits) from the DHT sensor... 
   int8 iIndex; 
   int8 iValue = 0; 

   for(iIndex = 0; iIndex < 8 ; iIndex++) 
   { 
      bExit = False; 
      set_timer0(0); 
      while (!input(DHT_IO) && !bExit); 
      delay_us(30); 

      if (input(DHT_IO)) 
      { 
         iValue = iValue |(1<<(7 - iIndex)); 
         bExit = False; 
         set_timer0(0); 
         while (input(DHT_IO) && !bExit); 
      } 
   } 

   return iValue;    
} 
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//                END Sensor Temperature & Humidity Function                                           //
/////////////////////////////////////////////////////////////////////////////////////////////////////////




void DHT_update(int Channel) {
//////////////////////
//      IN MAIN     //
//////////////////////
   int iByteIndex;
   int DHT_Data[5];
   int CRC;


      if (Channel == 1) { DHT_IO = DHT1_IO; }
      if (Channel == 2) { DHT_IO = DHT2_IO; }

      // Here we send the initialization sequence to "wake up" the DHT sensor, and put it into 
      // data transmission mode. 
      DHT_Start(); 

      // Here we read a total of 5 bytes of data (40 bits) from the DHT sensor. For the DHT-11 sensor, 
      // the integer data is contained in the 1st (Humidity), and 3rd (Temperature) bytes transmitted, 
      // and the 2nd and 4th decimal bytes are always zero (0). For the DHT-22 sensor, the integer data is  
      // is contained in the 1st (Humidity), and 3rd (Temperature) bytes transmitted, and the decimal 
      // data is contained in the 2nd (Humidity), and 4th (Temperature) bytes transmitted. The 5th byte 
      // contains the CRC byte for both sensor types. 
      for (iByteIndex = 0; iByteIndex < 5 ; iByteIndex++) 
      { 
         DHT_Data[iByteIndex] = DHT_ReadData(); 
      } 

      // Here we calculate the CRC by adding the 1st four bytes, and looking at only the lower 8 bits. 
      // This value should match the 5th byte sent by the sensor which is the CRC byte! 
      CRC = DHT_Data[0] + DHT_Data[1] + DHT_Data[2] + DHT_Data[3]; 
      CRC = CRC & 0xFF; 

      if (CRC != DHT_Data[4] && !bExit) 
      //   fprintf(PC, "CRC Error, expected: %Lu, and received: %u\n\r", CRC, DHT_Data[4]); 
      {  output_toggle(PIN_C2);
      }

      if (DHT_MODEL == DHT11) 
      { 
         DHT_humidity = DHT_Data[0]; 
            DHT_temperature = DHT_Data[2]; 
         //fprintf(PC, "Humidity: %ld%%\n\r", Humidity); 
         //fprintf(PC, "Temperature: %ldC\n\r", Temperature); 
      } 
      else 
      { 
            DHT_humidity = make16(DHT_Data[0], DHT_Data[1]); 
         DHT_temperature = make16(DHT_Data[2] & 0x7F, DHT_Data[3]);   //Strip off the sign bit first! 
            //Temperature /= 10; 
            if (DHT_Data[2] & 0x80) 
            DHT_temperature *= -1; 
         //fprintf(PC, "Humidity: %3.1w%%\n\r", Humidity); 
         //fprintf(PC, "Temperature: %3.1wC\n\r", Temperature); 
      } 
 
   
}


