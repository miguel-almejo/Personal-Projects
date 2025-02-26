#ifndef __TIMER1INTS_H__ 
#define __TIMER1INTS_H__
#include <stdint.h>


void Timer_Start(void);
uint32_t Time_Elapsed(void);
void Timer_1usDelay(uint8_t delay);
void Timer_Stop(void);
#endif