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

/**********************************************************************
**   PRIVATE CONSTANTS
**********************************************************************/
#define channel 1U /// Channel Midi of this device. must be different for each device

#define premiereNote 36U /// most left note of keyboard = C3 = Do 3eme Octave
#define velociteDefault 64U /// default velocity of keypressed
#define scanPeriod 1U ///scan every mS
#define delayOfNop 50U

/**********************************************************************
**   GLOBAL VARIABLES
**********************************************************************/
uint8_t previousKeys[8] = {0};
uint8_t inputImage[8] = {0};
uint8_t diffKey = 0 ;
elapsedMillis sinceLoop;


/**********************************************************************
**   FUNCTIONS
**********************************************************************/
inline void nop(void) __attribute__((always_inline)) {
	delayMicroseconds(delayOfNop);
}

inline uint8_t readKeys(void) __attribute__((always_inline)) {
	/// return concatenation of PB6,PB2,PB3,PB1,PF7,PF6,PF5,PF4
	return (((PINB & (1<<6)) << 1)|((PINB & (1<<2)) << 4)|((PINB & (1<<3)) << 2)|((PINB & (1<<1)) << 3)|((PINF & 0b11110000) >> 4));
}

inline void sendKeys(void) __attribute__((always_inline)) {
	// check key changes and send noteOn for that keys
        for (uint8_t i = 0 ; i < 8 ; i++) {
            diffKey = previousKeys[i] ^ inputImage[i]; // Xor = bits at 1 where key changed
            if (diffKey) { // if at least one key changed
                for (uint8_t j = 0 ; j < 8 ; j++){
                    if ((diffKey>>j) & 0b00000001){ // if that particular key changed
            	        usbMIDI.sendNoteOn(((i * 8) + j + premiereNote) , (~(inputImage[i]>>j) & 0b00000001) * velociteDefault , channel); 
				        /// noteOn if the key has changed and velocity velociteDefault ou 0 if On/Off
		            }
                }
			    previousKeys[i] = inputImage[i]; /// keep memory of key pressed
            }
        }
}

/**********************************************************************
**   SETUP - INITIALIZATION
**********************************************************************/
void setup(void) {
    ///Initialize Inputs PB6,PB2,PB3,PB1,PF7,PF6,PF5,PF4 = write 0
    DDRB &= 0b10110001;
	DDRF &= 0b00001111;
    ///Initialize Outputs PB5,PB4,PE6,PD7,PC6,PD4,PD0,PD1 as Inputs_pullup
    DDRB &= 0b11001111; /// Inputs
	PORTB |= 0b00110000; /// Pullups
	DDRD &= 0b01111100;/// Inputs
	PORTD |= 0b10000011; /// Pullups
	DDRE &= 0b10111111;/// Inputs
	PORTE |= 0b01000000; /// Pullups
    
}

/**********************************************************************
**   LOOP
**********************************************************************/
void loop(void) {
	if (sinceLoop >= scanPeriod) {
		
		sinceLoop = 0; ///reset timer
		
		/// first row PD1
		PORTD &= 0b11111101; /// Pullup OFF
		DDRD |=  0b00000010; /// Output
		nop();
		inputImage[0] = readKeys();
		DDRD &=  0b11111101; ///Input
		PORTD |= 0b00000010; /// Pullup
		
		/// second row PD0
		PORTD &= 0b11111110; /// Pullup OFF
		DDRD |=  0b00000001; /// Output
		nop();
		inputImage[1] = readKeys();
		DDRD &=  0b11111110; ///Input
		PORTD |= 0b00000001; /// Pullup
		
		/// third row PD4
		PORTD &= 0b11101111; /// Pullup OFF
		DDRD |=  0b00010000; /// Output
		nop();
		inputImage[2] = readKeys();
		DDRD &=  0b11101111; ///Input
		PORTD |= 0b00010000; /// Pullup
		
		/// fourth row PC6
		PORTC &= 0b10111111; /// Pullup OFF
		DDRC |=  0b01000000; /// Output
		nop();
		inputImage[3] = readKeys();
		DDRC &=  0b10111111; ///Input
		PORTC |= 0b01000000; /// Pullup
		
		/// fifth row PD7
		PORTD &= 0b01111111; /// Pullup OFF
		DDRD |=  0b10000000; /// Output
		nop();
		inputImage[4] = readKeys();
		DDRD &=  0b01111111; ///Input
		PORTD |= 0b10000000; /// Pullup
		
		/// sixth row PE6
		PORTE &= 0b10111111; /// Pullup OFF
		DDRE |=  0b01000000; /// Output
		nop();
		inputImage[5] = readKeys();
		DDRE &=  0b10111111; ///Input
		PORTE |= 0b01000000; /// Pullup
		
		/// seventh row PB4
		PORTB &= 0b11101111; /// Pullup OFF
		DDRB |=  0b00010000; /// Output
		nop();
		inputImage[6] = readKeys();
		DDRB &=  0b11101111; ///Input
		PORTB |= 0b00010000; /// Pullup
		
		/// eighth row PB5
		PORTB &= 0b11011111; /// Pullup OFF
		DDRB |=  0b00100000; /// Output
		nop();
		inputImage[7] = readKeys();
		DDRB &=  0b11011111; ///Input
		PORTB |= 0b00100000; /// Pullup
		
		sendKeys(); /// check and send MIDI messages
		usbMIDI.send_now(); /// flush USB buffer 
	}
}

//EOF
