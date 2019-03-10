/**********************************************************************
**
**   FILE NAME       :   Clavier_32u4_0.2.ino
**   DESCRIPTION     :   Fatar keyboard Management with Arduino Pro Micro clone
**   DATE CREATED    :   Sept 2017
**   DATE LAST MODIF.:
**   ASSOCIATED FILES:
**   NOTES :	
**   Documentation :
**   input is PD1, PD0, PD4, PC6, PD7, PE6, PB4, PB5 (bit 0..7)
**   output is PB6,PB2,PB3,PB1,PF7,PF6,PF5,PF4 (bit 0..7)
**
**   usbMIDI to send messages. See website to read messages https://www.pjrc.com/teensy/td_midi.html
**   usbMIDI.sendNoteOn(note, velocity, channel)
**   usbMIDI.sendNoteOff(note, velocity, channel)
**   usbMIDI.sendPolyPressure(note, pressure, channel)
**   usbMIDI.sendControlChange(control, value, channel)
**   usbMIDI.sendProgramChange(program, channel)
**   usbMIDI.sendAfterTouch(pressure, channel)
**   usbMIDI.sendPitchBend(value, channel)
**   usbMIDI.sendSysEx(length, array)
**
**   WARNINGS :
**
**   need teeOnArdu integrated to Arduino IDE for this code to work
**
**********************************************************************/

/**********************************************************************
**   INCLUDES
**********************************************************************/
#include "Arduino.h"
#include <EEPROM.h>

/**********************************************************************
**   PRIVATE CONSTANTS
**********************************************************************/
#define PREMIERE_NOTE 36U /// most left note of keyboard = C2 = Do 2eme Octave
#define VELOCITE_DEFAULT 64U /// default velocity of keypressed
#define SCAN_PERIOD 1U ///scan every mS
#define WAIT_TIME 100U

/**********************************************************************
**   GLOBAL VARIABLES
**********************************************************************/
uint8_t previousKeys[8] = {0};
uint8_t inputImage[8] = {0};
uint8_t diffKey = 0 ;
uint8_t channel = 1 ;
elapsedMillis sinceLoop;


/**********************************************************************
**   FUNCTIONS
**********************************************************************/
inline void wait() {
	delayMicroseconds(WAIT_TIME);
}

inline uint8_t readKeys() {
	/// return concatenation of PD1, PD0, PD4, PC6, PD7, PE6, PB4, PB5 (bit 0..7)
 uint8_t touches = 0;
 touches = ~((PIND>>1 & 1)<<0 | (PIND>>0 & 1)<<1 | (PIND>>4 & 1)<<2 | (PINC>>6 & 1)<<3 | (PIND>>7 & 1)<<4 | (PINE>>6 & 1)<<5 | (PINB>>4 & 1)<<6 | (PINB>>5 & 1)<<7  ); 
	return (touches);
}

inline void sendKeys() {
	// check key changes and send noteOn for that keys
    for (uint8_t i = 0 ; i < 8 ; i++) {
        diffKey = previousKeys[i] ^ inputImage[i]; // Xor = bits at 1 where key changed
        if (diffKey) { // if at least one key changed
            for (uint8_t j = 0 ; j < 8 ; j++){
                if ((diffKey>>j) & 0b00000001){ // if that particular key changed
          	        usbMIDI.sendNoteOn(((i * 8) + j + PREMIERE_NOTE) , ((inputImage[i]>>j) & 0b00000001) * VELOCITE_DEFAULT , channel); 
                    /*Serial.print("sendNoteOn ");
                    Serial.print(((i * 8) + j + PREMIERE_NOTE));
                    Serial.print(" ");
                    Serial.print( ((inputImage[i]>>j) & 0b00000001) * VELOCITE_DEFAULT);
                    Serial.print(" "); 
                    Serial.println(channel);*/
				    /// noteOn if the key has changed and velocity VELOCITE_DEFAULT ou 0 if On/Off
		        }
            }
            previousKeys[i] = inputImage[i]; /// keep memory of key pressed
        }
    }
}

inline void channelDefinition() {
	/// Check if keypressed at startup for channel definition. if not, look if channel is already set in EEPROM and valid
	uint8_t keyInit = 0;
	uint8_t lock = true;
	while (lock) {
	    firstRow();
	    keyInit = inputImage[0];
      /*Serial.println("3");
      Serial.println("2");
      Serial.println("1");
      Serial.print("keyInit =");
	    Serial.println(keyInit);*/
	    switch (keyInit) {
		    case 0:
		    if (EEPROM.read(0) > 0 && EEPROM.read(0) < 16) { /// if no key pressed check value stored in EEPROM to be valid
				lock = false; /// get out of while loop only if channel value is valid
			      }
			break;
		    case 1:
		    EEPROM.write(0,1); 
		    lock =false;
			break;
	        case 2:
		    EEPROM.write(0,2); 
		    lock =false;
			break;
		    case 4:
		    EEPROM.write(0,3);
		    lock =false;
			break;
		    case 8:
		    EEPROM.write(0,4); 
		    lock =false;
			break;
		    case 16:
		    EEPROM.write(0,5); 
		    lock =false;
			break;
		    case 32:
		    EEPROM.write(0,6); 
		    lock =false;
			break;
		    case 64:
		    EEPROM.write(0,7); 
		    lock =false;
			break;
		    case 128:
		    EEPROM.write(0,8); 
		    lock =false;
			break;
	    }
	}
	channel = EEPROM.read(0); /// channel value in EEPROM should be valid at this point.
  /*Serial.print("channel = ");
  Serial.println(channel);*/
}

inline void firstRow() {
	/// first row PB6
	PORTB &= ~(1 << 6); /// Pullup OFF
  DDRB |= 1<<6 ; /// Output
  wait();
	inputImage[0] = readKeys();
  DDRB &= ~(1<<6);///Input
  PORTB |= 1<<6; /// Pullup
}

inline void secondRow() {
    /// second row PB2
	PORTB &= ~(1 << 2); /// Pullup OFF
  DDRB |= 1<<2 ; /// Output
  wait();
  inputImage[1] = readKeys();
  DDRB &= ~(1<<2);///Input
  PORTB |= 1<<2; /// Pullup
}

inline void thirdRow() {
    /// third row PB3
	PORTB &= ~(1 << 3); /// Pullup OFF
  DDRB |= 1<<3 ; /// Output
  wait();
  inputImage[2] = readKeys();
  DDRB &= ~(1<<3);///Input
  PORTB |= 1<<3; /// Pullup
}

inline void fourthRow() {	
	/// fourth row PB1
	PORTB &= ~(1 << 1); /// Pullup OFF
  DDRB |= 1<<1 ; /// Output
  wait();
  inputImage[3] = readKeys();
  DDRB &= ~(1<<1);///Input
  PORTB |= 1<<1; /// Pullup
}

inline void fifthRow() {
	/// fifth row PF7
	PORTF &= ~(1 << 7); /// Pullup OFF
  DDRF |= 1<<7 ; /// Output
  wait();
  inputImage[4] = readKeys();
  DDRF &= ~(1<<7);///Input
  PORTF |= 1<<7; /// Pullup
}

inline void sixthRow() {
	/// sixth row PF6
	PORTF &= ~(1 << 6); /// Pullup OFF
  DDRF |= 1<<6 ; /// Output
  wait();
  inputImage[5] = readKeys();
  DDRF &= ~(1<<6);///Input
  PORTF |= 1<<6; /// Pullup
}

inline void seventhRow() {
	/// seventh row PF5
	PORTF &= ~(1 << 5); /// Pullup OFF
  DDRF |= 1<<5 ; /// Output
  wait();
  inputImage[6] = readKeys();
  DDRF &= ~(1<<5);///Input
  PORTF |= 1<<5; /// Pullup
}

inline void eighthRow() {
	/// eighth row PF4
  PORTF &= ~(1 << 4); /// Pullup OFF
  DDRF |= 1<<4 ; /// Output
  wait();
  inputImage[7] = readKeys();
  DDRF &= ~(1<<4);///Input
  PORTF |= 1<<4; /// Pullup
}
/**********************************************************************
**   SETUP - INITIALIZATION
**********************************************************************/
void setup(void) {
  //necessary to use pinMode to correctly initialyse pins 
  pinMode(2,INPUT_PULLUP);
  pinMode(3,INPUT_PULLUP);
  pinMode(4,INPUT_PULLUP);
  pinMode(5,INPUT_PULLUP);
  pinMode(6,INPUT_PULLUP);
  pinMode(7,INPUT_PULLUP);
  pinMode(8,INPUT_PULLUP);
  pinMode(9,INPUT_PULLUP);
//***
  pinMode(10,INPUT_PULLUP);
  pinMode(16,INPUT_PULLUP);
  pinMode(14,INPUT_PULLUP);
  pinMode(15,INPUT_PULLUP);
  pinMode(18,INPUT_PULLUP);
  pinMode(19,INPUT_PULLUP);
  pinMode(20,INPUT_PULLUP);
  pinMode(21,INPUT_PULLUP);
  //Serial.begin(9600);
    
channelDefinition();
  
}

/**********************************************************************
**   LOOP
**********************************************************************/
void loop(void) {
	if (sinceLoop >= SCAN_PERIOD) {
		
	  sinceLoop = 0; ///reset timer
		firstRow();
		secondRow();
		thirdRow();
		fourthRow();
		fifthRow();
		sixthRow();
		seventhRow();
		eighthRow();
		sendKeys(); /// check and send MIDI messages
		usbMIDI.send_now(); /// flush USB buffer 
		// MIDI Controllers should discard incoming MIDI messages.
		while (usbMIDI.read()) {
		// read & ignore incoming messages
		}
	}
}

//EOF
