#include "tm4c123gh6pm.h"
#include "PWM.h"

void PWM_PA76_Init(void){
    if ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOA)==0) {
		SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA;	// Activate A clocks
		while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOA)==0){};
	}
	
	GPIO_PORTA_AFSEL_R |= 0xC0;	// enable alt funct: PA76 for PWM
  GPIO_PORTA_PCTL_R &= ~0xFF000000;
  GPIO_PORTA_PCTL_R |= 0x55000000;
  GPIO_PORTA_DEN_R |= 0xC0;
	
	SYSCTL_RCGCPWM_R |= 0x02;	//Activate PWM1
	SYSCTL_RCC_R &= ~0x001E0000;
	
	PWM1_1_CTL_R = 0;	// re-loading down-counting mode
	PWM1_1_GENA_R |= 0xC8;	// low on LOAD, high on CMPA down
	PWM1_1_GENB_R |= 0xC08;// low on LOAD, high on CMPB down
	PWM1_1_LOAD_R = Period - 1;
  PWM1_1_CMPA_R = 0;
	PWM1_1_CMPB_R = 0;
	
	PWM1_1_CTL_R |= 0x00000001;	// Enable PWM1 Generator 1 in Countdown mode
	PWM1_ENABLE_R &= ~0x0000000C;	// Disable PA76:PWM1
}
void PWM_PB76_Init(void){
	if ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOB)==0) {
		SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;	// Activate B clocks
		while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOB)==0){};
	}
	
	GPIO_PORTB_AFSEL_R |= 0xC0;	// enable alt funct: PB76 for PWM
  GPIO_PORTB_PCTL_R &= ~0xFF000000;
  GPIO_PORTB_PCTL_R |= 0x44000000;
  GPIO_PORTB_DEN_R |= 0xC0;
	
	SYSCTL_RCGCPWM_R |= 0x01;	//Activate PWM0
	SYSCTL_RCC_R &= ~0x001E0000;
	
	PWM0_0_CTL_R = 0;	// re-loading down-counting mode
	PWM0_0_GENA_R |= 0xC8;	// low on LOAD, high on CMPA down
	PWM0_0_GENB_R |= 0xC08;// low on LOAD, high on CMPB down
	PWM0_0_LOAD_R = Period - 1;
  PWM0_0_CMPA_R = 0;
	PWM0_0_CMPB_R = 0;
	
	PWM0_0_CTL_R |= 0x00000001;	// Enable PWM0 Generator 0 in Countdown mode
	PWM0_ENABLE_R &= ~0x00000003;	// Disable PB76:PWM0
}
void PWM_PC54_init(void){
	if ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOC)==0) {
		SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOC;	// Activate C clocks
		while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOC)==0){};
	}
	
	GPIO_PORTC_AFSEL_R |= 0x30;	// enable alt funct: PC54 for PWM
  GPIO_PORTC_PCTL_R &= ~0x00FF0000;
  GPIO_PORTC_PCTL_R |= 0x00440000;
  GPIO_PORTC_DEN_R |= 0x30;
	
	SYSCTL_RCGCPWM_R |= 0x01;	//Activate PWM1
	SYSCTL_RCC_R &= ~0x001E0000;
	
	PWM0_3_CTL_R = 0;	// re-loading down-counting mode
	PWM0_3_GENA_R |= 0xC8;	// low on LOAD, high on CMPA down
	PWM0_3_GENB_R |= 0xC08;// low on LOAD, high on CMPB down
	PWM0_3_LOAD_R = Period - 1;
  PWM0_3_CMPA_R = 0;
	PWM0_3_CMPB_R = 0;
	
	PWM0_3_CTL_R |= 0x00000001;	// Enable PWM1 Generator 3 in Countdown mode
	PWM0_ENABLE_R &= ~0x000000C0;	// Disable PC54:PWM0
}

void PWM_PA76_Duty(unsigned long act_1, unsigned long act_2){
	PWM1_1_CMPA_R = act_1 - 1;	// PA6 count value when output rises
  PWM1_1_CMPB_R = act_2 - 1;	// PA7 count value when output rises
}
void PWM_PB76_Duty(unsigned long act_1, unsigned long act_2){
	PWM0_0_CMPA_R = act_1 - 1;	// PB6 count value when output rises
  PWM0_0_CMPB_R = act_2 - 1;	// PB7 count value when output rises
}
void PWM_PC54_Duty(unsigned long act_1, unsigned long act_2){
	PWM0_3_CMPA_R = act_1 - 1;	// PC4 count value when output rises
  PWM0_3_CMPB_R = act_2 - 1;	// PC5 count value when output rises
}