//Code for Arduino Major Scale midterm assignment
//By Patrick Evans 

//Include SPI Library
#include <SPI.h>

//Define Pins 8 and 9 with variable names
#define DAC 8
#define Sel 9

//Define Analog pin 0 as a variable name
const int shieldPot = A0;

//Define variable integers to be used later
int note, x, dur, freq, t, per, i, j, pot;

// Assign each note frequency at each octave
int Ao4 = 440.00;
int Bo4 = 493.88;
int Co4 = 261.63;
int Do4 = 293.67;
int Eo4 = 329.63;
int Fo4 = 349.23;
int Go4 = 392.00;
int Co5 = 523.25;

//Organize Notes into array, in order given in assignment
int noteScale[] = {Co4, Do4, Eo4, Fo4, Go4, Ao4, Bo4, Co5, Co5, Bo4, Ao4, Go4, Fo4, Eo4, Do4, Co4};

//Run void setup to declare earlier defined pins as outputs
void setup() {
  //Set Pins as outputs
  pinMode(Sel, OUTPUT);
  pinMode(DAC, OUTPUT);
  //Initialize SPI Bus
  SPI.begin();
}

//void playTone is used to play a given frequency for a given time, called upon in void loop
void playTone(int freq, int t) {
  long cycle;
  per = 500000/freq;
  cycle = ((long)freq*(long)t)/1000;
  
  for (i = 0; i <= cycle; i++) {
    DACIn(4921);
    delayMicroseconds(per);
    DACIn(0);
    delayMicroseconds(per);
  }
}

//Run void loop to cycle through each element of noteScale array and calls on playTone to play each element
void loop() {
  x = 0;
  while (x <= 15) {
    pot = analogRead(shieldPot) + 1;    //Read analog pin a0 which is the programmable potentiometer
    dur = pot * (1500/255);             //Scale the pot value to be max of 1.5 seconds
    note = noteScale[x];                //Select element from noteScale array
    playTone((int)note, dur);           //Call playTone with frequency and length inputs
    delay(150);                         //Delay between each loop to space notes
    x = x + 1;
  }
}

//void DACIn is used to configure the DAC
void DACIn(int obuffer) {
  obuffer |= 0b0111000000000000;
  char* config_char = (char*)&obuffer;
  PORTB &= 0b11111101;            //PORTB used (D Pins 8-13)
  SPI.transfer(config_char [1]);    
  SPI.transfer(config_char [0]);
  PORTB |= 0b00000010;
  PORTB &= 0b11111110;            //set DAC as low (min)
  PORTB |= 0b00000001;            //set DAC as high (max)
}
