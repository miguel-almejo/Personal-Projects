#define Period		16000
#define ON				Period * 0.80
#define OFF				Period * 0

// Ports used as hardware PWM to use with actuators
void PWM_PA76_Init(void);
void PWM_PB76_Init(void);
void PWM_PC54_init(void);

void PWM_PA76_Duty(unsigned long act_1, unsigned long act_2);
void PWM_PB76_Duty(unsigned long act_1, unsigned long act_2);
void PWM_PC54_Duty(unsigned long act_1, unsigned long act_2);