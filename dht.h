
// Define the PIN number to use for communication here

#define DHT1_IO   PIN_A2
#define DHT2_IO   PIN_A2


#define False     0
#define True      1

int DHT_IO=DHT1_IO;

int DHT_MODEL=DHT11;

int1 bExit;

int16 DHT_humidity;
signed int16 DHT_temperature;
int sensor_status;

void DHT_Start(void); 
int8 DHT_ReadData(void);
void DHT_update(int Channel);
