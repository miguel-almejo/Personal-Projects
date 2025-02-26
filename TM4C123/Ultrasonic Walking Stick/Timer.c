#include "tm4c123gh6pm.h"
#include "Timer.h"
#include <stdint.h>

#define one_ms		16

void Timer_Start(void){
  SYSCTL_RCGCTIMER_R |= 0x02;   // activate TIMER1
  TIMER1_CTL_R = 0x00000000;    // disable TIMER1A during setup
  TIMER1_CFG_R = 0x00000004;    // configure for 32-bit mode
  TIMER1_TAMR_R = 0x00000002;   // configure for periodic down-count mode
  TIMER1_TAILR_R = 0xFFFFFFFF;    // reload value
  NVIC_EN0_R = 1<<21;           // enable IRQ 19 in NVIC
  TIMER1_CTL_R = 0x00000001;    // enable TIMER1A
}

void Timer_Handler(void){
  TIMER1_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER1A timeout
}

uint32_t Time_Elapsed(void){
	return TIMER1_TAILR_R-TIMER1_TAV_R;
}

void Timer_1usDelay(uint8_t delay){
	SYSCTL_RCGCTIMER_R |= 0x02;   // activate TIMER1
	TIMER1_CTL_R = 0x00000000;    // disable TIMER1A during setup
  TIMER1_CFG_R = 0x00000004;    // configure for 32-bit mode
  TIMER1_TAMR_R = 0x00000002;   // configure for periodic down-count mode
  TIMER1_TAILR_R = delay*one_ms;    // reload value
  TIMER1_TAPR_R = 0;            // bus clock prescale
  TIMER1_ICR_R = 0x00000001;    // clear TIMER1A timeout flag
  TIMER1_CTL_R = 0x00000001;    // enable TIMER1A
	
	while((TIMER1_RIS_R & 0X01) == 0);
	TIMER1_ICR_R = 0x00000001;
}
	
void Timer_Stop(void){
	TIMER1_CTL_R = 0x00000000;
}
	