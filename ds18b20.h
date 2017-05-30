
// Define the PIN number to use for communication here
#define DS1820_IO          PIN_A2
#define DS_MAX_DEVICES     8



int8 ow_reset(void);
int8 read_bit(void);
void write_bit(int8 bitval); 
int8 read_byte(void); 
void write_byte(int8 val, int8 power_on); 
int8 ow_crc(int8 x); 
int8 Next(void); 
int8 First(void); 
void FindDevices(void); 
int8 Send_MatchRom(int8 actNumRom); 
void init_DS18B20();
void update_DS18B20();

//void testDS18B20();

