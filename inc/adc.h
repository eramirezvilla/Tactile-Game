//
// COMP-GENG 421 - Edgar Ramirez-Villa
//
// ADC (Analog-to-Digital Converter) module header file
//

#ifndef ADC_H_
#define ADC_H_

void AdcInit(void);
void AdcClock(void);
Bool AdcIsTwistedLeft(void);
void AdcClearIsTwistedLeft(void);
Bool AdcIsTwistedRight(void);
void AdcClearIsTwistedRight(void);

#endif // ADC_H_
