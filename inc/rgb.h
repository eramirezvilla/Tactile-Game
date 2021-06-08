// COMP-GENG 421 - Edgar Ramirez-Villa
//
// RGB LED module header file
//

#ifndef RGB_H_
#define RGB_H_

#include "gpio.h"


#define RGB_RED_ENA()		(GPIO_PB4_CLR())
#define RGB_RED_DIS()		(GPIO_PB4_SET())
#define RGB_RED_TOG()		(GPIO_PB4_TOG())

#define RGB_RED_PWM_MIN		 0
#define RGB_RED_PWM_MAX		10

#define RGB_GRN_ENA()		(GPIO_PC7_CLR())
#define RGB_GRN_DIS()		(GPIO_PC7_SET())
#define RGB_GRN_TOG()		(GPIO_PC7_TOG())

#define RGB_GRN_PWM_MIN		 0
#define RGB_GRN_PWM_MAX		10

#define RGB_BLU_ENA()		(GPIO_PA9_CLR())
#define RGB_BLU_DIS()		(GPIO_PA9_SET())
#define RGB_BLU_TOG()		(GPIO_PA9_TOG())

#define RGB_BLU_PWM_MIN		 0
#define RGB_BLU_PWM_MAX		10


void	RgbClock(void);
void	RgbRedPwmSet(int PwmSetting);
int		RgbRedPwmGet(void);
void	RgbGrnPwmSet(int PwmSetting);
int		RgbGrnPwmGet(void);
void	RgbBluPwmSet(int PwmSetting);
int		RgbBluPwmGet(void);

#endif	// RGB_H_
