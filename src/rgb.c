// COMP-GENG 421 - Edgar Ramirez-Villa
//
// RGB LED module
//

#include "main.h"
#include "rgb.h"


#define RED_ST_OFF		0
#define RED_ST_ON		1

#define GRN_ST_OFF		0
#define GRN_ST_ON		1

#define BLU_ST_OFF		0
#define BLU_ST_ON		1


static int	RedPwmSetting = 0;
static int	GrnPwmSetting = 0;
static int	BluPwmSetting = 0;


void RgbClock(void)
{
	static int	RedState = RED_ST_OFF;
	static int	RedPwmCount = 0;
	static int	GrnState = GRN_ST_OFF;
	static int	GrnPwmCount = 0;
	static int	BluState = BLU_ST_OFF;
	static int	BluPwmCount = 0;

	RedPwmCount++;
	switch(RedState)
	{
	case RED_ST_OFF:
		if (RedPwmCount == RGB_RED_PWM_MAX)
		{
			RedPwmCount = 0;
			if (RedPwmSetting != 0)
			{
				RGB_RED_ENA();
				RedState = RED_ST_ON;
				break;
			}
		}
		break;

	case RED_ST_ON:
		if (RedPwmCount >= RedPwmSetting)
		{
			if (RedPwmSetting == RGB_RED_PWM_MAX)
			{
				RedPwmCount = 0;
			}
			else
			{
				RGB_RED_DIS();
				RedState = RED_ST_OFF;
				break;
			}
		}
		break;
	}

	GrnPwmCount++;
	switch(GrnState)
	{
	case GRN_ST_OFF:
		if (GrnPwmCount == RGB_GRN_PWM_MAX)
		{
			GrnPwmCount = 0;
			if (GrnPwmSetting != 0)
			{
				RGB_GRN_ENA();
				GrnState = GRN_ST_ON;
				break;
			}
		}
		break;

	case GRN_ST_ON:
		if (GrnPwmCount >= GrnPwmSetting)
		{
			if (GrnPwmSetting == RGB_GRN_PWM_MAX)
			{
				GrnPwmCount = 0;
			}
			else
			{
				RGB_GRN_DIS();
				GrnState = GRN_ST_OFF;
				break;
			}
		}
		break;
	}

	BluPwmCount++;
	switch(BluState)
	{
	case BLU_ST_OFF:
		if (BluPwmCount == RGB_BLU_PWM_MAX)
		{
			BluPwmCount = 0;
			if (BluPwmSetting != 0)
			{
				RGB_BLU_ENA();
				BluState = BLU_ST_ON;
				break;
			}
		}
		break;

	case BLU_ST_ON:
		if (BluPwmCount >= BluPwmSetting)
		{
			if (BluPwmSetting == RGB_BLU_PWM_MAX)
			{
				BluPwmCount = 0;
			}
			else
			{
				RGB_BLU_DIS();
				BluState = BLU_ST_OFF;
				break;
			}
		}
		break;
	}
}


void RgbRedPwmSet(int PwmSetting)
{
	if (PwmSetting == 0)
	{
		RedPwmSetting = 0;
		return;
	}
	if (PwmSetting < RGB_RED_PWM_MIN)
	{
		PwmSetting = RGB_RED_PWM_MIN;
	}
	if (PwmSetting > RGB_RED_PWM_MAX)
	{
		PwmSetting = RGB_RED_PWM_MAX;
	}
	RedPwmSetting = PwmSetting;
}


int RgbRedPwmGet(void)
{
	return RedPwmSetting;
}


void RgbGrnPwmSet(int PwmSetting)
{
	if (PwmSetting == 0)
	{
		GrnPwmSetting = 0;
		return;
	}
	if (PwmSetting < RGB_GRN_PWM_MIN)
	{
		PwmSetting = RGB_GRN_PWM_MIN;
	}
	if (PwmSetting > RGB_GRN_PWM_MAX)
	{
		PwmSetting = RGB_GRN_PWM_MAX;
	}
	GrnPwmSetting = PwmSetting;
}


int RgbGrnPwmGet(void)
{
	return GrnPwmSetting;
}


void RgbBluPwmSet(int PwmSetting)
{
	if (PwmSetting == 0)
	{
		BluPwmSetting = 0;
		return;
	}
	if (PwmSetting < RGB_BLU_PWM_MIN)
	{
		PwmSetting = RGB_BLU_PWM_MIN;
	}
	if (PwmSetting > RGB_BLU_PWM_MAX)
	{
		PwmSetting = RGB_BLU_PWM_MAX;
	}
	BluPwmSetting = PwmSetting;
}


int RgbBluPwmGet(void)
{
	return BluPwmSetting;
}
