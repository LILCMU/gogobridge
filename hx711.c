
    // "yield" is not implemented as noop in older Arduino Core releases, so let's define it.
    // See also: https://stackoverflow.com/questions/34497758/what-is-the-secret-of-the-arduino-yieldfunction/34498165#34498165
void HX711_yield(void) {
   delay_us(100);
}


HX711_init(byte gain) {
   HX711_begin(gain);
}


void HX711_begin(byte gain) {

//!   pinMode(PD_SCK, OUTPUT);
//!   pinMode(DOUT, INPUT);

   HX711_set_gain(gain);
}

bool HX711_is_ready() {
   return (input(DOUT) == LOW);
}

void HX711_set_gain(byte gain) {
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

int32 HX711_read() {

   int32 buffer;
   int i;

   // wait for the chip to become ready
   while (!HX711_is_ready()) {
      // Will do nothing on Arduino but prevent resets of ESP8266 (Watchdog Issue)
      HX711_yield();
   }

//!   uint8_t data[3] = { 0 };
//!   uint8_t filler = 0x00;

   for (i=24;i>0;i--) {
      output_high(PD_SCK);
      delay_us(100);
      output_low(PD_SCK);
      delay_us(100);
      if (input(DOUT)) {   bit_set(buffer,i-1); }
      else             {  bit_clear(buffer,i-1);}
   }
   
//!   data[2] = buffer>>16;
//!   data[1] = (buffer>>8) & 255;
//!   data[0] = buffer & 255;
   
  
   // pulse the clock pin 24 times to read the data
//!   data[2] = shiftIn(DOUT, PD_SCK, MSBFIRST);
//!   data[1] = shiftIn(DOUT, PD_SCK, MSBFIRST);
//!   data[0] = shiftIn(DOUT, PD_SCK, MSBFIRST);
//!
   // set the channel and the gain factor for the next reading using the clock pin
   for (i = 0; i < GAIN; i++) {
      output_bit(PD_SCK, HIGH);
      output_bit(PD_SCK, LOW);
   }

//!   // Replicate the most significant bit to pad out a 32-bit signed integer
//!   if (data[2] & 0x80) {
//!      filler = 0xFF;
//!   } else {
//!      filler = 0x00;
//!   }
//!
//!   // Construct a 32-bit signed integer
//!   value = ( static_cast<unsigned long>(filler) << 24
//!         | static_cast<unsigned long>(data[2]) << 16
//!         | static_cast<unsigned long>(data[1]) << 8
//!         | static_cast<unsigned long>(data[0]) );
//!
//!   return static_cast<long>(value);

   if (bit_test(buffer, 23)) { buffer = buffer | (0xff<<24); }
   
   return buffer;



}

int32 HX711_read_average(byte times) {
   int32 sum = 0;
   for (byte i = 0; i < times; i++) {
      sum += HX711_read();
      HX711_yield();
   }
   return sum / times;
}

double HX711_get_value(byte times) {
   return HX711_read_average(times) - OFFSET;
}

float HX711_get_units(byte times) {
   return HX711_get_value(times) / SCALE;
}

void HX711_tare(byte times) {
   double sum = HX711_read_average(times);
   HX711_set_offset(sum);
}

void HX711_set_scale(float scale) {
   SCALE = scale;
}

float HX711_get_scale() {
   return SCALE;
}

void HX711_set_offset(int32 offset) {
   OFFSET = offset;
}

int32 HX711_get_offset() {
   return OFFSET;
}

void HX711_power_down() {
   output_bit(PD_SCK, LOW);
   output_bit(PD_SCK, HIGH);
}

void HX711_power_up() {
   output_bit(PD_SCK, LOW);
}

