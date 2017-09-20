#define premiereNote 36U
#define velociteDefault 64U
#define channel 1U
//scan every x mS
#define scanPeriod 1U
#define nop() delayMicroseconds(50)


#define Reading7 ((PINB & (1<<6)) << 1) // pin 10 as bit 7 of Reading value
#define Reading6 ((PINB & (1<<2)) << 4) // pin 16 as bit 6 of Reading value
#define Reading5 ((PINB & (1<<3)) << 2) // pin 14 as bit 5 of Reading value
#define Reading4 ((PINB & (1<<1)) << 3) // pin 15 as bit 4 of Reading value
#define Reading30 ((PINF & b11110000) >> 4) // pin A0,A1,A2,A3 as bit 3,2,1,0 of Reading value
#define Reading (Reading7 | Reading6 | Reading5 | Reading4 | Reading30) // concatenation to one byte

uint8_t previousKeys[8] = {0};
uint8_t inputImage[8] = {0};
uint8_t diffKey = 0 ;
unsigned long previousMillis = 0 ;

/*#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif
*/
