#ifndef CECS447PROJECT1PART2_H
#define CECS447PROJECT1PART2_H

// define music note data structure 
struct Note {
  unsigned char tone_index;
  unsigned char delay;
};
typedef const struct Note NTyp;

// Constant definitions
#define NUM_SONGS 3

// Function prototypes
void play_a_song(NTyp notetab[]);

void play_tone(void);
void Music_Init(void);
void Sound_Init(unsigned long period);
void DAC_Init(void);
void PortD_InIt(void);
void Sound_stop(void);
void DAC_Out(unsigned long data);

unsigned char is_music_on(void);

void Switch_Init(void);

// Initialize SysTick with busy wait running at bus clock.
void SysTick_Init(void);
void SysTick_start(void);
void SysTick_stop(void);
void SysTick_Set_Current_Note(unsigned long n_value);

#endif