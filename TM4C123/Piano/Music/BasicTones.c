// This is an example program to show music programming on the LaunchPad.
// You can run this program without modification.
// The program will play 8 basic notes in selected C scale: Happy Birthday, 
// doe ray mi fa so la ti 
// C   D   E  F  G  A  B
// in forward and backward order and repeat forever.
// Hardware connection: 
// Positive logic Speaker/Headset is connected to PA2.
// Authors: Min He
// Date: August 28, 2018

// 1. Pre-processor Directives Section
// Constant declarations to access port registers using 
// symbolic names instead of addresses
#include "tm4c123gh6pm.h"
#include "SysTick.h"

#define TEMPO 2 // each tone uses the same duration 

// 2. Declarations Section
// Constants

// initial values for piano major tones: 
// Assume system clock is 16MHz.
const unsigned long Tone_Tab[] = 
// Note name: C, D, E, F, G, A, B, C'
// Offset:0, 1, 2, 3, 4, 5, 6, 7
{30534,27211,24242,22923,20408,18182,16194,15289}; // C4 Major notes
//{15289,13621,12135,11454,10204,9091,8099,7645}; // C5 Major notes
//{7645,6810,6067,5727,5102,4545,4050,3822}; // C6 Major notes

//   Function Prototypes
void Speaker_Init(void);
void Delay(void);

void Switch_Init(void);
int SW1(void);
int SW2(void);

extern void EnableInterrupts(void);

// 3. Subroutines Section
// MAIN: Mandatory for a C Program to be executable
int main(void){
  unsigned char i,j;
	unsigned char currentTone = 0;
	
  Speaker_Init();
  SysTick_Init();
	Switch_Init();
  EnableInterrupts();  // SysTick uses interrupts
	// i=8 or i++
	for (i=0;i<8;i++) {
		// load the inital value for current note
		SysTick_Set_Current_Note(Tone_Tab[i]);
		SysTick_start();
		for(j=0;j<TEMPO;j++) // play the note for specified duration: tempo control
			Delay();
		SysTick_stop();
	}
	// i=8 or i--;
	for (i=8;i>0;i--) {  // i=8 to 1
		 
		// load the inital value for current note
		SysTick_Set_Current_Note(Tone_Tab[i-1]);
		SysTick_start();
		for (j=0;j<TEMPO;j++) // play the note for specified duration
			Delay();
		SysTick_stop();
	}
	while(1){
		if(SW1()){
			if(currentTone < sizeof(Tone_Tab)/sizeof(Tone_Tab[0])-1){
				currentTone++;
				SysTick_Set_Current_Note(Tone_Tab[currentTone]);
				SysTick_start();
				for(j=0;j<TEMPO;j++){
					Delay();
				}
				SysTick_stop();
		}
  }
		else if(SW2()){
			if(currentTone > 0){
				currentTone--;
				SysTick_Set_Current_Note(Tone_Tab[currentTone]);
				SysTick_start();
				for(j=0;j<TEMPO;j++){
					Delay();
				}
				SysTick_stop();
			}
		}
		else{
			SysTick_stop();
		}
  }
}

// Subroutine to wait 0.1 sec
// Inputs: None
// Outputs: None
// Notes: ...
void Delay(void){
	unsigned long volatile time;
  time = 727240*50/91;  // 0.1sec
  while(time){
		time--;
  }
}
// Make PA2 an output, enable digital I/O, ensure alt. functions off
void Speaker_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x01;           // 1) activate clock for Port A
  delay = SYSCTL_RCGC2_R;           // allow time for clock to start
                                    // 2) no need to unlock PA2
  GPIO_PORTA_PCTL_R &= ~0x00000F00; // 3) regular GPIO
  GPIO_PORTA_AMSEL_R &= ~0x04;      // 4) disable analog function on PA2
  GPIO_PORTA_DIR_R |= 0x04;         // 5) set direction to output
  GPIO_PORTA_AFSEL_R &= ~0x04;      // 6) regular port function
  GPIO_PORTA_DEN_R |= 0x04;         // 7) enable digital port
  GPIO_PORTA_DR8R_R |= 0x04;        // 8) optional: enable 8 mA drive on PA2 to increase the voice volume
}
void Switch_Init(void){
  SYSCTL_RCGC2_R |= 0x00000020; // (a) activate clock for port F
  while((SYSCTL_PRGPIO_R&0x20)==0){};
  GPIO_PORTF_LOCK_R = 0x4C4F434B; // unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x11;         // allow changes to PF4,0
  GPIO_PORTF_DIR_R &= ~0x11;    // (c) make PF4,0 in (built-in button)
  GPIO_PORTF_AFSEL_R &= ~0x11;  //     disable alt funct on PF4,0
  GPIO_PORTF_DEN_R |= 0x11;     //     enable digital I/O on PF4,0
  GPIO_PORTF_PCTL_R &= ~0x000F000F; //  configure PF4,0 as GPIO
  GPIO_PORTF_AMSEL_R &= ~0x11;  //     disable analog functionality on PF4,0
  GPIO_PORTF_PUR_R |= 0x11;     //     enable weak pull-up on PF4,0
  }
int SW1(void){
	return (GPIO_PORTF_DATA_R&0x10) == 0;
}

int SW2(void){
	return (GPIO_PORTF_DATA_R&0x01) == 0;
}
