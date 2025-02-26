#include "tm4c123gh6pm.h"
#include "Timer.h"
#include "UART.h"
#include "PLL.h"
#include "GPIO.h"
#include "PWM.h"
#include "Right_Sensor.h"
#include "Left_Sensor.h"
#include "Front_Sensor.h"
//#include "Bottom_Sensor.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


#define mc_len 					0.0625 // length of one machine cycle in microsecond for 16MHz clock
#define speed_sound 		0.0135043 // inches per micro-second

extern void EnableInterrupts(void);
void delayMs(uint32_t delay);

static volatile uint8_t flag = 0;
static volatile uint32_t DistanceR, DistanceF, DistanceL = 0; //DistanceB

int main(void){
	PLL_Init();
	
	PortA_Init();
	PortB_Init();
	PortC_Init();
//	PortD_Init();
	
	PA5_Init();
	PE2_Init();
	PB4_Init();
	PE1_Init();
	PE5_Init();
	PD3_Init();
	
	PWM_PA76_Init();
	PWM_PB76_Init();
	PWM_PC54_init();
	PWM_PA76_Duty(ON, ON);
	PWM_PB76_Duty(ON, ON);
	PWM_PC54_Duty(ON, OFF);
	
	UART_Init(false, false);
  EnableInterrupts();
	UART_OutString((uint8_t *)"\n\rInitializing Sensors.....\n\r");
  while(1){
		flag = 0;
		DistanceL, DistanceF, DistanceR  = 0; //DistanceB
		
		Front_Sensor_Init(); 	// initiiate the front sensor
		delayMs(20);
		Right_Sensor_Init(); 	// initiate the right sensor
		delayMs(20);
		Left_Sensor_Init(); 	// initiate the left sensor
		delayMs(20);
//		Bottom_Sensor_Init(); // initiate the bottom sensor
//		delayMs(20);
		
    while(!flag);
		
		// Right Sensor: R
		// In Braille:|.  |
		//						|.. |
		//						|.  |
		if((DistanceR != 0) && (DistanceR < 10)){
			IN_1|= Actuator1;
			PWM1_ENABLE_R |= 0x04;	// Enable PA6:PWM1
			
			IN_3|= Actuator3;
			IN_5|= Actuator5;
			IN_6|= Actuator6;
			PWM0_ENABLE_R |= 0x43;	// Enable PC4:PWM0 & PB76:PWM0
			
		}
		// Left Sensor: L
		// In Braille:|.  |
		//						|.  |
		//						|.  |
		else if((DistanceL != 0) && (DistanceL < 10)){
			IN_1|= Actuator1;
			PWM1_ENABLE_R |= 0x04;	// Enable PA6:PWM1
			
			IN_3|= Actuator3;
			IN_5|= Actuator5;
			PWM0_ENABLE_R |= 0x41;	// Enable PC4:PWM0 & PB6:PWM0
		}
	
		// Front Sensor: F
		// In Braille:|.. |
		//						|.	|
		//						|   |
		else if((DistanceF != 0) && (DistanceF < 10)){
			IN_1|= Actuator1;
			IN_2|= Actuator2;
			PWM1_ENABLE_R |= 0x0C;	// Enable PA76:PWM1
			
			IN_5|= Actuator5;
			PWM0_ENABLE_R |= 0x01;	// Enable PB6:PWM0
		}
		
//		// Bottom Sensor: B
//		// In Braille:|. |
//		//						|. |
//		//						|  |
//		else if((DistanceB != 0) && (DistanceB < 10) && (DistanceB < DistanceL) && (DistanceB < DistanceF) && (DistanceB < DistanceR)){
//			IN_1 |= Actuator1;
//			PWM1_ENABLE_R |= 0x04;	// Enable PA76:PWM1
//		
//			IN_5 |= Actuator5;
//			PWM0_ENABLE_R |= 0x01;	// Enable PB6:PWM0
//		}
//		
		else{
			// Turn Every Actuator off
			PWM1_ENABLE_R &= ~0x0C;	// Disable PA76:PWM1
			PWM0_ENABLE_R &= ~0xC3;	// Disable PC54:PWM0 & PB6:PWM0
		}	
  }
}

void GPIOPortA_Handler(void){
	if(ECHO_RPin==ECHO_RIGHT){
		Timer_Start();
		}
	else{
		Timer_Stop(); // stops the timing
		DistanceR = (uint32_t)(Time_Elapsed()*speed_sound*mc_len)/2;
		flag = 1;
		
		UART_OutString((uint8_t *)"Right Sensor: ");
		UART_OutUDec(DistanceR);
		UART_OutString((uint8_t *)"in\n\r");
		
	}
  GPIO_PORTA_ICR_R = 0x08;      //acknowledge flag
}
void GPIOPortB_Handler(void){
	if(ECHO_LPin==ECHO_LEFT){
		Timer_Start();
		}
	else{
		Timer_Stop(); // stops the timing
		DistanceL = (uint32_t)(Time_Elapsed()*speed_sound*mc_len)/2;
		flag = 1;
		
		UART_OutString((uint8_t *)"Left Sensor: ");
		UART_OutUDec(DistanceL);
		UART_OutString((uint8_t *)"in\n\r");
	}	
  GPIO_PORTB_ICR_R = 0x08;      //acknowledge flag
}

void GPIOPortC_Handler(void){
	if(ECHO_FPin==ECHO_FRONT){
		Timer_Start();
		}
	else{
		Timer_Stop(); // stops the timing
		DistanceF = (uint32_t)(Time_Elapsed()*speed_sound*mc_len)/2;
		flag = 1;
		
		UART_OutString((uint8_t *)"Front Sensor: ");
		UART_OutUDec(DistanceF);
		UART_OutString((uint8_t *)"in\n\r");
	}
  GPIO_PORTC_ICR_R = 0x80;      //acknowledge flag
}

//void GPIOPortD_Handler(void){
//	if(ECHO_BPin==ECHO_BOTTOM){
//		Timer_Start();
//		}
//	else{
//		Timer_Stop(); // stops the timing
//		DistanceB = (uint32_t)(Time_Elapsed()*speed_sound*mc_len)/2;
//		flag = 1;
//		
//		UART_OutString((uint8_t *)"Bottom Sensor: ");
//		UART_OutUDec(DistanceB);
//		UART_OutString((uint8_t *)"in\n\r");
//	}
//  GPIO_PORTD_ICR_R = 0x80;      //acknowledge flag
//}

void delayMs(uint32_t delay){
    volatile uint32_t i;
    for (i = 0; i < delay * 16000; i++);
}	