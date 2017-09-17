#define Reading7 ((PINB & (1<<6)) << 1)
#define Reading6 ((PINB & (1<<2)) << 4)
#define Reading5 ((PINB & (1<<3)) << 2)
#define Reading4 ((PINB & (1<<1)) << 3)
#define Reading57 ((PINF & b11110000) >> 4)
#define Reading (Reading7 | Reading6 | Reading5 | Reading4 | Reading57)
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif
