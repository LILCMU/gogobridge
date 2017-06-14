//!#DEFINE HX711_SELF_TEST


#IFDEF HX711_SELF_TEST
   #include <gogobridge.h>
   #include <hx711.h>
#ENDIF

void HX711_output_pulse() {
      output_high(PD_SCK);  delay_us(1);  // pulse width is 1 us typical as per the datasheet
      output_low(PD_SCK); delay_us(1);
}
    
    // "yield" is not implemented as noop in older Arduino Core releases, so let's define it.
    // See also: https://stackoverflow.com/questions/34497758/what-is-the-secret-of-the-arduino-yieldfunction/34498165#34498165
void HX711_yield(void) {
   delay_us(100);
}


HX711_init(byte gain=HX711_GAIN) {
   HX711_begin(gain);
}


void HX711_begin(byte gain=HX711_GAIN) {

//!   pinMode(PD_SCK, OUTPUT);
//!   pinMode(DOUT, INPUT);

   HX711_set_gain(gain);
}

bool HX711_is_ready() {
   return (input(DOUT) == LOW);
}

void HX711_set_gain(byte gain=HX711_GAIN) {
   switch (gain) {
      case 128:      // channel A, gain factor 128
         GAIN = 1;
         break;
      case 64:      // channel A, gain factor 64
         GAIN = 3;
         break;
      case 32:      // channel B, gain factor 32
         GAIN = 2;
         break;
   }

   output_bit(PD_SCK, LOW);
   HX711_read();
}

signed int32 HX711_read() {

   signed int32 buffer=0;
   int i;

   // wait for the chip to become ready
   while (!HX711_is_ready()) {
      // Will do nothing on Arduino but prevent resets of ESP8266 (Watchdog Issue)
      HX711_yield();
   }


   for (i=24;i>0;i--) {
      HX711_output_pulse();
      if (input(DOUT)) {   bit_set(buffer,i-1); }
      else             {  bit_clear(buffer,i-1);}
   }
   
   // set the channel and the gain factor for the next reading using the clock pin
   for (i = 0; i < GAIN; i++) {
      HX711_output_pulse();
   }

   if (bit_test(buffer, 23)) { buffer = buffer | ((int32)0xff<<24); }
   
   return buffer;



}

// HX711_read() itself executes quite fast (~0.5ms) 
// but is stuck at HX711_is_ready() for a long time
// (~90 ms). Should try to improve this.

signed int32 HX711_read_average(byte times=10) {
   signed int32 sum = 0;
   for (byte i = 0; i < times; i++) {
      sum += HX711_read();
      HX711_yield();
   }
   return sum / times;
}

float HX711_get_value(byte times=1) {
   return HX711_read_average(times) - OFFSET;
}

float HX711_get_units(byte times = 1) {
   return HX711_get_value(times) / SCALE;
}

void HX711_tare(byte times=10) {
   float sum = HX711_read_average(times);
   HX711_set_offset(sum);
}

void HX711_set_scale(float scale=1.0) {
   SCALE = scale;
}

float HX711_get_scale() {
   return SCALE;
}

void HX711_set_offset(float offset=0) {
   OFFSET = offset;
}

float HX711_get_offset() {
   return OFFSET;
}

void HX711_power_down() {
   output_bit(PD_SCK, LOW);
   output_bit(PD_SCK, HIGH);
}

void HX711_power_up() {
   output_bit(PD_SCK, LOW);
}

#IFDEF HX711_SELF_TEST

void main() {

   float weight;

  printf("HX711 Demo\r\n");
  delay_ms(2000);
  printf("Start in 5 secs\r\n");
  delay_ms(5000);

  printf("Initializing the scale\r\n");

  HX711_begin();
  HX711_set_scale(12125);                      // this value is obtained by calibrating the scale with known weights; see the README for details

  // Best to print an average reading out once before calling tare()
  printf("First read average = %Ld\r\n", HX711_read_average(10));
  HX711_tare();
  
  while(1) {
   weight= HX711_get_units(10);
   printf("get units = %f\r\n", weight );
   printf("%u - %lu\r\n", (int)weight, (int16)(weight*10)%10);
   printf("ofset = %f\r\n", OFFSET);
   printf("read average = %Ld\r\n", HX711_read_average(10));
   delay_ms(1000);
  
  }
  

}



#ENDIF

