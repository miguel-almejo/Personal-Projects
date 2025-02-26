#define TRIGGER_BPin 			(*((volatile unsigned long *)0x40007100))  // PD6	
#define ECHO_BPin 				(*((volatile unsigned long *)0x40007200))  // PD7

#define TRIGGER_BOTTOM 	0x40   // PD6
#define ECHO_BOTTOM 		0x80   // PD7

void Bottom_Sensor_Init();