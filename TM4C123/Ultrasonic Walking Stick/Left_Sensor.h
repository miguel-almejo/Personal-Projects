#define TRIGGER_LPin 			(*((volatile unsigned long *)0x40005010))  // PB2	
#define ECHO_LPin 				(*((volatile unsigned long *)0x40005020))  // PB3

#define TRIGGER_LEFT		0x04   // PB2
#define ECHO_LEFT				0x08   // PB3

void Left_Sensor_Init();
