/**********************************************************************
**
**   FILE NAME       :   Clavier_32u4_0.1.ino
**   DESCRIPTION     :   Fatar keyboard Management with Arduino Pro Micro clone
**   DATE CREATED    :   Sept 2017
**   DATE LAST MODIF.:
**   ASSOCIATED FILES:
**   NOTES :	
**   Documentation :
**   input is PB6,PB2,PB3,PB1,PF7,PF6,PF5,PF4
**   output is PB5,PB4,PE6,PD7,PC6,PD4,PD0,PD1
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
#define PREMIERE_NOTE 36U /// most left note of keyboard = C3 = Do 3eme Octave
#define VELOCITE_DEFAULT 64U /// default velocity of keypressed
#define SCAN_PERIOD 1U ///scan every mS
#define WAIT_TIME 50U

/**********************************************************************
**   GLOBAL VARIABLES
**********************************************************************/
uint8_t previousKeys[8] = {255,255,255,255,255,255,255,255};
uint8_t inputImage[8] = {0};
uint8_t diffKey = 0 ;
uint8_t channel = 0 ;
elapsedMillis sinceLoop;


/**********************************************************************
**   FUNCTIONS
**********************************************************************/
inline void wait() {
	delayMicroseconds(WAIT_TIME);
}

inline uint8_t readKeys() {
	/// return concatenation of PB6,PB2,PB3,PB1,PF7,PF6,PF5,PF4
	return (((PINB & (1<<6)) << 1)|((PINB & (1<<2)) << 4)|((PINB & (1<<3)) << 2)|((PINB & (1<<1)) << 3)|((PINF & 0b11110000) >> 4));
}

inline void sendKeys() {
	// check key changes and send noteOn for that keys
    for (uint8_t i = 0 ; i < 8 ; i++) {
        diffKey = previousKeys[i] ^ inputImage[i]; // Xor = bits at 1 where key changed
        if (diffKey) { // if at least one key changed
            for (uint8_t j = 0 ; j < 8 ; j++){
                if ((diffKey>>j) & 0b00000001){ // if that particular key changed
          	        usbMIDI.sendNoteOn(((i * 8) + j + PREMIERE_NOTE) , (~(inputImage[i]>>j) & 0b00000001) * VELOCITE_DEFAULT , channel); 
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
	uint8_t lock = 1;
	while (lock) {
	    firstRow();
	    keyInit = ~inputImage[0];
	    switch (keyInit) {
		    case 0:
		    if (EEPROM.read(0) > 0 && EEPROM.read(0) < 16) { /// if no key pressed check value stored in EEPROM to be valid
				lock = 0; /// get out of while loop only if channel value is valid
			}
			break;
		    case 1:
		    EEPROM.write(0,1); 
		    lock =0;
			break;
	        case 2:
		    EEPROM.write(0,2); 
		    lock =0;
			break;
		    case 4:
		    EEPROM.write(0,3);
		    lock =0;
			break;
		    case 8:
		    EEPROM.write(0,4); 
		    lock =0;
			break;
		    case 16:
		    EEPROM.write(0,5); 
		    lock =0;
			break;
		    case 32:
		    EEPROM.write(0,6); 
		    lock =0;
			break;
		    case 64:
		    EEPROM.write(0,7); 
		    lock =0;
			break;
		    case 128:
		    EEPROM.write(0,8); 
		    lock =0;
			break;
	    }
	}
	channel = EEPROM.read(0); /// channel value in EEPROM should be valid at this point.
}

inline void firstRow() {
	/// first row PD1
	PORTD &= 0b11111101; /// Pullup OFF
	DDRD |=  0b00000010; /// Output
	wait();
	inputImage[0] = readKeys();
	DDRD &=  0b11111101; ///Input
	PORTD |= 0b00000010; /// Pullup
}

inline void secondRow() {
    /// second row PD0
	PORTD &= 0b11111110; /// Pullup OFF
	DDRD |=  0b00000001; /// Output
	wait();
	inputImage[1] = readKeys();
	DDRD &=  0b11111110; ///Input
	PORTD |= 0b00000001; /// Pullup
}

inline void thirdRow() {
    /// third row PD4
	PORTD &= 0b11101111; /// Pullup OFF
	DDRD |=  0b00010000; /// Output
	wait();
	inputImage[2] = readKeys();
	DDRD &=  0b11101111; ///Input
	PORTD |= 0b00010000; /// Pullup
}

inline void fourthRow() {	
	/// fourth row PC6
	PORTC &= 0b10111111; /// Pullup OFF
	DDRC |=  0b01000000; /// Output
	wait();
	inputImage[3] = readKeys();
	DDRC &=  0b10111111; ///Input
	PORTC |= 0b01000000; /// Pullup
}

inline void fifthRow() {
	/// fifth row PD7
	PORTD &= 0b01111111; /// Pullup OFF
	DDRD |=  0b10000000; /// Output
	wait();
	inputImage[4] = readKeys();
	DDRD &=  0b01111111; ///Input
	PORTD |= 0b10000000; /// Pullup
}

inline void sixthRow() {
	/// sixth row PE6
	PORTE &= 0b10111111; /// Pullup OFF
	DDRE |=  0b01000000; /// Output
	wait();
	inputImage[5] = readKeys();
	DDRE &=  0b10111111; ///Input
	PORTE |= 0b01000000; /// Pullup
}

inline void seventhRow() {
	/// seventh row PB4
	PORTB &= 0b11101111; /// Pullup OFF
	DDRB |=  0b00010000; /// Output
	wait();
	inputImage[6] = readKeys();
	DDRB &=  0b11101111; ///Input
	PORTB |= 0b00010000; /// Pullup
}

inline void eighthRow() {
	/// eighth row PB5
	PORTB &= 0b11011111; /// Pullup OFF
	DDRB |=  0b00100000; /// Output
	wait();
	inputImage[7] = readKeys();
	DDRB &=  0b11011111; ///Input
	PORTB |= 0b00100000; /// Pullup
}
/**********************************************************************
**   SETUP - INITIALIZATION
**********************************************************************/
void setup(void) {
    ///Initialize Inputs PB6,PB2,PB3,PB1,PF7,PF6,PF5,PF4 = write 0
    DDRB &= 0b10110001;
	DDRF &= 0b00001111;
    ///Initialize Outputs PB5,PB4,PE6,PD7,PC6,PD4,PD0,PD1 as Inputs_pullup
    DDRB &= 0b11001111; /// Inputs PORTB
	PORTB |= 0b00110000; /// Pullups
	DDRD &= 0b01111100;/// Inputs
	PORTD |= 0b10000011; /// Pullups
	DDRE &= 0b10111111;/// Inputs
	PORTE |= 0b01000000; /// Pullups
    
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
	}
}

//EOF
