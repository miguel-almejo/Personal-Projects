#define TRIGGER_RPin 			(*((volatile unsigned long *)0x40004010))  // PA2
#define ECHO_RPin 				(*((volatile unsigned long *)0x40004020))  // PA3	

#define TRIGGER_RIGHT		0x04   // PA2
#define ECHO_RIGHT 			0x08   // PA3

void Right_Sensor_Init();