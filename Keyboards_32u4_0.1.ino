/**********************************************************************
**
**   FILE NAME       :   Clavier_32u4_0.1.ino
**   DESCRIPTION     :   Fatar keyboard Management with Arduino Pro Micro clone
**   DATE CREATED    :   Sept 2017
**   DATE LAST MODIF.:
**   ASSOCIATED FILES:
**   NOTES :	
**   Documentation usbMIDI to send messages. See website to read messages https://www.pjrc.com/teensy/td_midi.html
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
#define nop() delayMicroseconds(50)

/**********************************************************************
**   GLOBAL VARIABLES
**********************************************************************/
uint8_t previousKeys[8] = {0};
uint8_t inputImage[8] = {0};
uint8_t diffKey = 0 ;
unsigned long previousMillis = 0 ;

/**********************************************************************
**   FUNCTIONS
**********************************************************************/
inline void readKeys(void){

}

/**********************************************************************
**   SETUP - INITIALIZATION
**********************************************************************/
void setup(void) {
///Initialize Inputs

///Initialize Outputs

    
}

/**********************************************************************
**   LOOP
**********************************************************************/
void loop(void) {
    
}

//EOF