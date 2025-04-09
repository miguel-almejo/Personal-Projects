#include "tm4c123gh6pm.h"
#include "CECS447Project1Part2.h"
// Index for notes used in music scores
#define C3 0
#define D3 1
#define E3 2
#define F3 3
#define G3 4
#define A3 5
#define B3 6
#define C4 0+7
#define D4 1+7
#define E4 2+7
#define F4 3+7
#define G4 4+7
#define A4 5+7
#define B4 6+7
#define C5 0+2*7
#define D5 1+2*7
#define E5 2+2*7
#define F5 3+2*7
#define G5 4+2*7
#define A5 5+2*7
#define B5 6+2*7
#define C6 7+2*7
#define D6 1+3*7
#define E6 2+3*7
#define F6 3+3*7
#define G6 4+3*7
#define A6 5+3*7
#define B6 6+3*7
#define PAUSE 255				// assume there are less than 255 tones used in any song
#define MAX_NOTES 50  // assume maximum number of notes in any song is 50. You can change this value if you add a long song.
#define SPEAKER (*((volatile unsigned long *)0x40004020)) // define SPEAKER connects to PA3: 
#define BUTTONS (*((volatile unsigned long *)0x4000703C)) // PD3-0
#define Speaker_Toggle     			0x00000008  // Used to toggle the speaker output
#define NVIC_ST_CTRL_COUNT      0x00010000  // Count flag
#define NVIC_ST_CTRL_CLK_SRC    0x00000004  // Clock Source
#define NVIC_ST_CTRL_INTEN      0x00000002  // Interrupt enable
#define NVIC_ST_CTRL_ENABLE     0x00000001  // Counter mode
#define NUM_VALs 64
#define piano_mode 0
#define auto_play 1


// 2. Declarations Section
unsigned int i;
unsigned int current_song_index = 0;//index for determining which song to play
unsigned int octave = 0;//index for determining which octave in array to use
unsigned int octave_change[3] = {0, 7, 14};//array that holds octave values
unsigned int press = 0;//flag to determine if Port D buttons are being pressed or released
unsigned long button;//holds Port D button values
unsigned int Index;//index for sine wave
unsigned char tone_index = 0;//index for determining which music note to play
unsigned int mode_index; //index for determining which mode device is in
unsigned int mode[2] = {piano_mode, auto_play};//array holding mode values
unsigned int current_mode;//flag used for PortF handler
	unsigned int current_song; 
// initial values for piano major notes: assume SysTick clock is 16MHz.
const unsigned char SineWave[64] = {32,35,38,41,44,47,49,52,54,56,58,
																		59,61,62,62,63,63,63,62,62,61,59,
																		58,56,54,52,49,47,44,41,38,35,32,
																		29,26,23,20,17,15,12,10,8,6,5,
																		3,2,2,1,1,1,2,2,3,5,6,
																		8,10,12,15,17,20,23,26,29};
const unsigned long tonetab[] =
// initial values for three major notes for 16MHz system clock
// Note name: C, D, E, F, G, A, B
// Offset:0, 1, 2, 3, 4, 5, 6
{61069*2,54422*2,48484*2,45846*2,40816*2,36364*2,32388*2, //  C3 Major notes
	30534*2,27211*2,24242*2,22923*2,20408*2,18182*2,16194*2,// C4 Major notes
15289*2,13621*2,12135*2,11454*2,10204*2,9091*2,8099*2,// C5 Major notes
7645*2,6810*2,6067*2,5727*2,5102*2,4545*2,4050*2}; // C6 Major notes
 
 NTyp Score_Tab[][MAX_NOTES] =
// score table for Twinkle Twinkle Little Stars
{{C3,4,C3,4,G3,4,G3,4,A3,4,A3,4,G3,8,F3,4,F3,4,E3,4,E3,4,D3,4,D3,4,C3,8, 
 G3,4,G3,4,F3,4,F3,4,E3,4,E3,4,D3,8,G3,4,G3,4,F3,4,F3,4,E3,4,E3,4,D3,8, 
 C3,4,C3,4,G3,4,G3,4,A3,4,A3,4,G3,8,F3,4,F3,4,E3,4,E3,4,D3,4,D3,4,C3,8,0,0},

// score table for Happy Birthday
   {G3,2,G3,2,A3,4,G3,4,C4,4,B3,4,
  PAUSE,4,  G3,2,G3,2,A3,4,G3,4,D4,4,C4,4,
  PAUSE,4,  G3,2,G3,2,G4,4,E4,4,C4,4,B3,4,A3,8, 
	PAUSE,4,  F4,2,F4,2, E4,4,C4,4,D4,4,C4,8,0,0},

// score table for Mary Had A Little Lamb
{E3, 4, D3, 4, C3, 4, D3, 4, E3, 4, E3, 4, E3, 8, 
 D3, 4, D3, 4, D3, 8, E3, 4, G3, 4, G3, 8,
 E3, 4, D3, 4, C3, 4, D3, 4, E3, 4, E3, 4, E3, 8, 
 D3, 4, D3, 4, E3, 4, D3, 4, C3, 8, 0, 0 }};

static unsigned long c_note;
// 3. Function Prototypes
extern void EnableInterrupts(void);
extern void WaitForInterrupt(void);
extern void DisableInterrupts(void);
void Delay(void); 

// 4. Subroutines Section
// MAIN: Mandatory for a C Program to be executable

int main(void){
  DisableInterrupts();
	Switch_Init();
	PortD_InIt();
	DAC_Init();
	
  
	EnableInterrupts();  // SysTick uses interrupts
  
  while(1){
		if(current_mode == auto_play) {
			play_a_song(Score_Tab[current_song_index]);
		}
		else{
			WaitForInterrupt();
		}
	}
}
		

void play_a_song(NTyp notetab[])
{
	unsigned char i=0, j;
	current_mode = mode[mode_index];
	current_song = current_song_index;
	while ((notetab[i].delay) && (current_mode == auto_play) && (current_song == current_song_index)) {
		if (notetab[i].tone_index==PAUSE) // index = 255 indicate a pause: stop systick
			SysTick_stop(); // silence tone, turn off SysTick timer
		else {
			Sound_Init(tonetab[notetab[i].tone_index+octave_change[octave]]/NUM_VALs); // Note to play
		}
		
		// tempo control: 
		// play current note for duration 
		// specified in the music score table
		for (j=0;j<notetab[i].delay;j++) 
			Delay();
		
		SysTick_stop();
		i++;  // move to the next note
	}
	
	// pause after each play
	for (j=0;j<3;j++) 
		Delay();
}

// Subroutine to wait 0.1 sec
// Inputs: None
// Outputs: None
// Notes: ...
void Delay(void){
	unsigned long volatile time;
  time = 727240*20/91;  // 0.1sec for 16MHz
  while(time){
		time--;
  }
}

// Subroutine to initialize port F pins for the two onboard switches
// enable PF4 and PF0 for SW1 and SW2 respectively with falling edge interrupt enabled.
// Priority 5
// Inputs: None
// Outputs: None
void Switch_Init(void){ 
	SYSCTL_RCGC2_R |= 0x00000020; // (a) activate clock for port F
  GPIO_PORTF_LOCK_R = 0x4C4F434B; // unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x1F;         // allow changes to PF4,0
  GPIO_PORTF_DIR_R &= ~0x11;    // (c) make PF4,0 in (built-in button)
  GPIO_PORTF_AFSEL_R &= ~0x11;  //     disable alt funct on PF4,0
  GPIO_PORTF_DEN_R |= 0x11;     //     enable digital I/O on PF4,0
  GPIO_PORTF_PCTL_R &= ~0x000F000F; //  configure PF4,0 as GPIO
  GPIO_PORTF_AMSEL_R &= ~0x11;  //     disable analog functionality on PF4,0
  GPIO_PORTF_PUR_R |= 0x11;     //     enable weak pull-up on PF4,0
  GPIO_PORTF_IS_R &= ~0x11;     // (d) PF4,PF0 is edge-sensitive
  GPIO_PORTF_IBE_R &= ~0x11;    //     PF4,PF0 is not both edges
  GPIO_PORTF_IEV_R &= ~0x11;    //     PF4,PF0 falling edge event
  GPIO_PORTF_ICR_R = 0x11;      // (e) clear flags 4,0
  GPIO_PORTF_IM_R |= 0x11;      // (f) arm interrupt on PF4,PF0
  NVIC_PRI7_R = (NVIC_PRI7_R&0xFF0FFFFF)|0x00800000; // (g) bits:23-21 for PORTF, set priority to 5
  NVIC_EN0_R = 0x40000000;      // (h) enable interrupt 30 in NVIC //
}

void PortD_InIt(void) {
	SYSCTL_RCGC2_R |= 0x00000008; // (a) activate clock for port D
	GPIO_PORTD_CR_R |= 0x0F;         // allow changes to PD3-0
	GPIO_PORTD_DIR_R &= ~0x0F;    // (c) make PD3-0 in
	GPIO_PORTD_AFSEL_R &= ~0x0F;  //     disable alt funct on PD3-0
	GPIO_PORTD_DEN_R |= 0x0F;     //     enable digital I/O on PD3-0
	GPIO_PORTD_PCTL_R &= ~0x0000FFFF; //  configure PD3-0 as GPIO
	GPIO_PORTD_AMSEL_R &= ~0x0F;  //     disable analog functionality on PD3-0
	
	GPIO_PORTD_IS_R &= ~0x0F;     // (d) PD3-0 is edge-sensitive
  GPIO_PORTD_IBE_R |= 0x0F;    //      PD3-0 is both edges
  GPIO_PORTD_ICR_R = 0x0F;      // (e) clear flags 3-0
  GPIO_PORTD_IM_R |= 0x0F;      // (f) arm interrupt on PD3-0
  NVIC_PRI0_R = (NVIC_PRI0_R&0x0FFFFFFF)|0x80000000; // (g) bits:31-29 for PORTF, set priority to 5
  NVIC_EN0_R = 0x00000008;      // (h) enable interrupt 30 in NVIC
}

void Sound_Init(unsigned long period){
  Index = 0;
  NVIC_ST_CTRL_R = 0;         // disable SysTick during setup
  NVIC_ST_RELOAD_R = period-1;// reload value
  NVIC_ST_CURRENT_R = 0;      // any write to current clears it
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x40000000; // priority 2      
  NVIC_ST_CTRL_R = 0x0007;  // enable SysTick with core clock and interrupts
}

void DAC_Init(void){unsigned long volatile delay;
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB; // activate port B
  delay = SYSCTL_RCGC2_R;    // allow time to finish activating
  GPIO_PORTB_AMSEL_R &= ~0x3F;      // no analog 
  GPIO_PORTB_PCTL_R &= ~0x00FFFFFF; // regular function
  GPIO_PORTB_DIR_R |= 0x3F;      // make PB5-0 out
  GPIO_PORTB_AFSEL_R &= ~0x3F;   // disable alt funct on PB5-0
  GPIO_PORTB_DEN_R |= 0x3F;      // enable digital I/O on PB5-0
  GPIO_PORTB_DR8R_R |= 0x3F;        // enable 8 mA drive on PB5-0
}
  
// ISR for PORTF
void GPIOPortF_Handler(void){
	for(i=0;i<=200000;i++){}//delay for button press to stabilize
	if(GPIO_PORTF_RIS_R&0x10){  // SW1 pressed
		if(current_mode == auto_play) {
			current_song_index	                                                                                                               = 0;
		}
		GPIO_PORTF_ICR_R = 0x10;
    mode_index = (mode_index + 1) % 2; //index value for mode to be in
		current_mode = mode[mode_index]; //determine if in piano or auto play mode
	}
	if(GPIO_PORTF_RIS_R&0x01) { //SW2 pressed
		GPIO_PORTF_ICR_R = 0x01;  // acknowledge interrupt
		if(current_mode == piano_mode) {
			octave = (octave + 1)%3; //next octave
		}
		if(current_mode == auto_play) {
			current_song_index = (current_song_index + 1)%3; //next song
		}
	}
}
	
void GPIOPortD_Handler(void) {
	button = GPIO_PORTD_DATA_R & 0x0F;
	if (GPIO_PORTD_RIS_R & 0x01)
	{  
		GPIO_PORTD_ICR_R = 0x01;  // Acknowledges Button Press 
		if((button == 0x01) && (press == 0) && (current_mode == piano_mode)) { //in piano mode and PD0 is being pressed
			press = 1;
			Sound_Init(tonetab[C3+octave_change[octave]]/NUM_VALs);
		}
		if((button != 0x01) && (press == 1)) { //button no longer being pressed
			press = 0;
			SysTick_stop();
		}
		
	}
	if (GPIO_PORTD_RIS_R & 0x02)
	{  
		GPIO_PORTD_ICR_R = 0x02;  // Acknowledges Button Press 
		if((button == 0x02) && (press == 0)&& (current_mode == piano_mode)) { //in piano mode and PD1 is being pressed
			press = 1;
			Sound_Init(tonetab[D3+octave_change[octave]]/NUM_VALs);
		}
		if((button != 0x02) && (press == 1)) {
			press = 0;
			SysTick_stop();
		}
		
	}
	if (GPIO_PORTD_RIS_R & 0x04)
	{  
		GPIO_PORTD_ICR_R = 0x04;  // Acknowledges Button Press 
		if((button == 0x04) && (press == 0)&& (current_mode == piano_mode)){ //in piano mode and PD2 is being pressed
			press = 1;
			Sound_Init(tonetab[E3+octave_change[octave]]/NUM_VALs);
		}
		if((button != 0x04) && (press == 1)) {
			press = 0;
			SysTick_stop();
		}
		
	}
	if (GPIO_PORTD_RIS_R & 0x08)
	{  
		GPIO_PORTD_ICR_R = 0x08;  // Acknowledges Button Press 
		if((button == 0x08) && (press == 0)&& (current_mode == piano_mode)) { //in piano mode and PD3 is being pressed
			press = 1;
			Sound_Init(tonetab[F3+octave_change[octave]]/NUM_VALs);
		}
		if((button != 0x08) && (press == 1)) {
			press = 0;
			SysTick_stop();
		}
		
	}
}
// Initialize SysTick with busy wait running at bus clock.

void SysTick_start(void)
{
  NVIC_ST_RELOAD_R = c_note;  // maximum reload value
  NVIC_ST_CURRENT_R = 0;                // any write to current clears it
	NVIC_ST_CTRL_R = NVIC_ST_CTRL_ENABLE+
	                 NVIC_ST_CTRL_CLK_SRC+NVIC_ST_CTRL_INTEN;
}
void SysTick_stop(void)
{
	  NVIC_ST_CTRL_R = 0;
}

// Interrupt service routine, 
// frequency is determined by current tone being played
void Sound_stop(void)
{
	  NVIC_ST_CTRL_R = 0;
}

// **************DAC_Out*********************
// output to DAC
// Input: 3-bit data, 0 to 7 
// Output: none
void DAC_Out(unsigned long data){
  GPIO_PORTB_DATA_R = data;
}

// Interrupt service routine
// Executed every 12.5ns*(period)
void SysTick_Handler(void){
  GPIO_PORTF_DATA_R ^= 0x08;     // toggle PF3, debugging
  Index = (Index+1)%64;        // 16 samples for each period
	GPIO_PORTB_DATA_R = SineWave[Index]; // output to DAC: 3-bit data
}