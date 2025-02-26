#include "Right_Sensor.h"
#include "tm4c123gh6pm.h"
#include "Timer.h"

void Right_Sensor_Init(){
	TRIGGER_RPin &= ~TRIGGER_RIGHT;
	Timer_1usDelay(2);
	TRIGGER_RPin |= TRIGGER_RIGHT;
	Timer_1usDelay(10);
	TRIGGER_RPin &= ~TRIGGER_RIGHT;
}
