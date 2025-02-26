#define TRIGGER_FPin 			(*((volatile unsigned long *)0x40006100))  // PC6	
#define ECHO_FPin 				(*((volatile unsigned long *)0x40006200))  // PC7

#define TRIGGER_FRONT 	0x40   // PC6
#define ECHO_FRONT 			0x80   // PC7

void Front_Sensor_Init();