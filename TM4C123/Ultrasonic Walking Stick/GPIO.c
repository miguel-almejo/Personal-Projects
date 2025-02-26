#include "tm4c123gh6pm.h"
#include <stdio.h>
#include <stdint.h>
#include "GPIO.h"

void PortA_Init(void){ 
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA;           // 1) activate clock for Port A
	while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOA)!=SYSCTL_RCGC2_GPIOA){}; // wait for clock to start
  GPIO_PORTA_PCTL_R &= ~0x0000FF00; //regular GPIO
  GPIO_PORTA_AMSEL_R &= (uint32_t)~0x0C;      //disable analog function
  GPIO_PORTA_DIR_R &= ~0x08;        //PA3: echo pin, input
  GPIO_PORTA_DIR_R |= 0x04;         //PA2: trigger pin, output
  GPIO_PORTA_AFSEL_R &= ~0x0C;      //regular port function
  GPIO_PORTA_DEN_R |= 0x0C;         //enable digital port
  GPIO_PORTA_IS_R &= ~0x08;         //PA3 is edge-sensitive
  GPIO_PORTA_IBE_R |= 0x08;         //PA3 is both edges
  GPIO_PORTA_IEV_R &= ~0x08;        //PA3 both edge event
  GPIO_PORTA_ICR_R = 0x08;          //clear flag 6
  GPIO_PORTA_IM_R |= 0x08;          //arm interrupt on PA3
  NVIC_PRI0_R = (NVIC_PRI0_R&0xFFFF1FFF)|0x00004000; //priority 3
  NVIC_EN0_R = 0x00000001;          //enable Port A edge interrupt
}

void PortB_Init(void){ 
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;           // 1) activate clock for Port B
	while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOB)!=SYSCTL_RCGC2_GPIOB){}; // wait for clock to start
  GPIO_PORTB_PCTL_R &= ~0x0000FF00; //regular GPIO
  GPIO_PORTB_AMSEL_R &= (uint32_t)~0x0C;      //disable analog function
  GPIO_PORTB_DIR_R &= ~0x08;        //PB3: echo pin, input
  GPIO_PORTB_DIR_R |= 0x04;         //PB2: trigger pin, output
  GPIO_PORTB_AFSEL_R &= ~0x0C;      //regular port function
  GPIO_PORTB_DEN_R |= 0x0C;         //enable digital port
  GPIO_PORTB_IS_R &= ~0x08;         //PB3 is edge-sensitive
  GPIO_PORTB_IBE_R |= 0x08;         //PB3 is both edges
  GPIO_PORTB_IEV_R &= ~0x08;        //PB3 both edge event
  GPIO_PORTB_ICR_R = 0x08;          //clear flag 6
  GPIO_PORTB_IM_R |= 0x08;          //arm interrupt on PB3
  NVIC_PRI0_R = (NVIC_PRI0_R&0xFFFF1FFF)|0x00004000; //priority 3
  NVIC_EN0_R = 0x00000002;          //enable Port B edge interrupt
}

void PortC_Init(void){ 
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOC;           // 1) activate clock for Port C
	while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOC)!=SYSCTL_RCGC2_GPIOC){}; // wait for clock to start
  GPIO_PORTC_PCTL_R &= ~0xFF000000; //regular GPIO
  GPIO_PORTC_AMSEL_R &= (uint32_t)~0xC0;      //disable analog function
  GPIO_PORTC_DIR_R &= ~0x80;        //PC7:echo pin, input
  GPIO_PORTC_DIR_R |= 0x40;         //PC6:trigger pin, output
  GPIO_PORTC_AFSEL_R &= ~0xC0;      //regular port function
  GPIO_PORTC_DEN_R |= 0xC0;         //enable digital port
  GPIO_PORTC_IS_R &= ~0x80;         //PC7 is edge-sensitive
  GPIO_PORTC_IBE_R |= 0x80;         //PC7 is both edges
  GPIO_PORTC_IEV_R &= ~0x80;        //PC7 both edge event
  GPIO_PORTC_ICR_R = 0x80;          //clear flag 6
  GPIO_PORTC_IM_R |= 0x80;          //arm interrupt on PC7
  NVIC_PRI0_R = (NVIC_PRI0_R&0xFFFF1FFF)|0x00004000; //priority 3
  NVIC_EN0_R = 0x00000004;          //enable Port C edge interrupt
}

void PortD_Init(void){ 
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD;           // 1) activate clock for Port C
	while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOD)!=SYSCTL_RCGC2_GPIOD){}; // wait for clock to start
	GPIO_PORTD_LOCK_R |= 0x4C4F434B;		//unlock PD7
	GPIO_PORTD_CR_R |= 0xC0;						//allow changes to PD7-6
  GPIO_PORTD_PCTL_R &= ~0xFF000000; 	//regular GPIO
  GPIO_PORTD_AMSEL_R &= (uint32_t)~0xC0;      //disable analog function
  GPIO_PORTD_DIR_R &= ~0x80;        //PD7:echo pin, input
  GPIO_PORTD_DIR_R |= 0x40;         //PD6:trigger pin, output
  GPIO_PORTD_AFSEL_R &= ~0xC0;      //regular port function
  GPIO_PORTD_DEN_R |= 0xC0;         //enable digital port
  GPIO_PORTD_IS_R &= ~0x80;         //PD7 is edge-sensitive
  GPIO_PORTD_IBE_R |= 0x80;         //PD7 is both edges
  GPIO_PORTD_IEV_R &= ~0x80;        //PD7 both edge event
  GPIO_PORTD_ICR_R = 0x80;          //clear flag 6
  GPIO_PORTD_IM_R |= 0x80;          //arm interrupt on PD7
  NVIC_PRI0_R = (NVIC_PRI0_R&0xFFFF1FFF)|0x00004000; //priority 3
  NVIC_EN0_R = 0x00000008;          //enable Port D edge interrupt
}

void PA5_Init(void){
	if ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOA)==0) {
		SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA;	// Activate B clocks
		while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOA)==0){};
	}
		
  GPIO_PORTA_AMSEL_R &= ~0x20;
	GPIO_PORTA_AFSEL_R &= ~0x20;
  GPIO_PORTA_PCTL_R &= ~0x00F00000; 
	GPIO_PORTA_DIR_R |= 0x20; // output on pin
  GPIO_PORTA_DEN_R |= 0x20;	// enable digital I/O on pin
}
void PE2_Init(void){
	if ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOE)==0) {
		SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE;	// Activate B clocks
		while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOE)==0){};
	}
		
  GPIO_PORTE_AMSEL_R &= ~0x04;
	GPIO_PORTE_AFSEL_R &= ~0x04;
  GPIO_PORTE_PCTL_R &= ~0x00000F00; 
	GPIO_PORTE_DIR_R |= 0x04; // output on pin
  GPIO_PORTE_DEN_R |= 0x04;	// enable digital I/O on pin
}
void PB4_Init(void){
	if ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOB)==0) {
		SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;	// Activate B clocks
		while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOB)==0){};
	}
		
  GPIO_PORTB_AMSEL_R &= ~0x10;
	GPIO_PORTB_AFSEL_R &= ~0x10;
  GPIO_PORTB_PCTL_R &= ~0x000F0000; 
	GPIO_PORTB_DIR_R |= 0x10; // output on pin
  GPIO_PORTB_DEN_R |= 0x10;	// enable digital I/O on pin
}
void PE1_Init(void){
	if ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOE)==0) {
		SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE;	// Activate E clocks
		while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOE)==0){};
	}
		
  GPIO_PORTE_AMSEL_R &= ~0x02;
	GPIO_PORTE_AFSEL_R &= ~0x02;
  GPIO_PORTE_PCTL_R &= ~0x000000F0; 
	GPIO_PORTE_DIR_R |= 0x02; // output on pin
  GPIO_PORTE_DEN_R |= 0x02;	// enable digital I/O on pin
}
void PE5_Init(void){
	if ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOE)==0) {
		SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE;	// Activate E clocks
		while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOE)==0){};
	}
		
  GPIO_PORTE_AMSEL_R &= ~0x20;
	GPIO_PORTE_AFSEL_R &= ~0x20;
  GPIO_PORTE_PCTL_R &= ~0x00F00000; 
	GPIO_PORTE_DIR_R |= 0x20; // output on pin
  GPIO_PORTE_DEN_R |= 0x20;	// enable digital I/O on pin
}
void PD3_Init(void){
	if ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOD)==0) {
		SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD;	// Activate B clocks
		while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOD)==0){};
	}
		
  GPIO_PORTD_AMSEL_R &= ~0x08;
	GPIO_PORTD_AFSEL_R &= ~0x08;
  GPIO_PORTD_PCTL_R &= ~0x0000F000; 
	GPIO_PORTD_DIR_R |= 0x08; // output on pin
  GPIO_PORTD_DEN_R |= 0x08;	// enable digital I/O on pin
}