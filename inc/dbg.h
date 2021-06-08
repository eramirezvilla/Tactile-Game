//
// COMP-GENG 421 - Edgar Ramirez-Villa
//
// Debug module header file
//

#ifndef DBG_H_
#define DBG_H_

#include "gpio.h"


// PC4 -> Debug pin
#define DBG_CLR()	(GPIO_PC4_CLR())
#define DBG_SET()	(GPIO_PC4_SET())
#define DBG_TOG()	(GPIO_PC4_TOG())

#endif	// DBG_H_
