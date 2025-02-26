#include "tm4c123gh6pm.h"
#include <stdio.h>
#include <stdint.h>

// Ports used to connect to LN298 motor driver to actuators
#define IN_1		(*((volatile unsigned long *)0x40004080))	//PA5
#define IN_2		(*((volatile unsigned long *)0x40024010))	//PE2
#define IN_3		(*((volatile unsigned long *)0x40005040))	//PB4
#define IN_4		(*((volatile unsigned long *)0x40024008))	//PE1
#define IN_5		(*((volatile unsigned long *)0x40024080))	//PE5
#define IN_6		(*((volatile unsigned long *)0x40007020))	//PD3

// Values that turn the acutators on
#define Actuator1		0x20	//PA5
#define Actuator2		0x04	//PE2
#define Actuator3		0x10	//PB4
#define Actuator4		0x02	//PE1
#define Actuator5		0x20	//PE5
#define Actuator6		0x08	//PD3

// Ports used for SRF05 Ultrasonic Sensors
void PortA_Init(void);
void PortB_Init(void);
void PortC_Init(void);
void PortD_Init(void);

// Specific Ports used to turn on actuators  
void PA5_Init(void);
void PE2_Init(void);
void PB4_Init(void);
void PE1_Init(void);
void PE5_Init(void);
void PD3_Init(void);