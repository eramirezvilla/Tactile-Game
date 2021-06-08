// COMP-GENG 421 - Edgar Ramirez-Villa
//
// Analog-to-Digital Converter (ADC) module
//

#include <stdlib.h>

#include "main.h"
#include "adc.h"


#define KNOB_ANGLE_MIN	0
#define KNOB_ANGLE_MAX	240

#define KNOB_LEFT		LL_ADC_CHANNEL_0
#define KNOB_RIGHT		LL_ADC_CHANNEL_1

#define READING_MIN		0
#define READING_MAX		0xFFF			// 12 bits
#define READING_OFFSET	8				// added to ADC reading to achieve full-scale

#define TWIST_ANGLE_TH_TRUE		33		// degrees, for twist state to go true
#define TWIST_ANGLE_TH_FALSE	27		// degrees, for twist state to go false

#define TWISTED_LEFT_ST_FALSE	0
#define TWISTED_LEFT_ST_TRUE	1

#define TWISTED_RIGHT_ST_FALSE	0
#define TWISTED_RIGHT_ST_TRUE	1


static Bool	IsTwistedLeft = FALSE;		// left knob twisted?
static Bool	IsTwistedRight = FALSE;		// right knob twisted?

static int TwistedLeftState = TWISTED_LEFT_ST_FALSE;
static int TwistedRightState = TWISTED_RIGHT_ST_FALSE;

static int LeftStartPos;
static int RightStartPos;


static int KnobAngle(int Knob);


void AdcInit(void)
{
	// Enable the GPIOA port clock
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);

	// Configure PA0 as ADC_IN0 (left knob), PA1 as ADC_IN1 (right knob)
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_0, LL_GPIO_MODE_ANALOG);
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_1, LL_GPIO_MODE_ANALOG);

	// Enable the clock for ADC1
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);

	// Set the conversion trigger source to software start
	LL_ADC_REG_SetTriggerSource(ADC1, LL_ADC_REG_TRIG_SOFTWARE);

	// Configure for single conversion mode of a single channel
	LL_ADC_REG_SetContinuousMode(ADC1, LL_ADC_REG_CONV_SINGLE);
	LL_ADC_REG_SetSequencerLength(ADC1, LL_ADC_REG_SEQ_SCAN_DISABLE);
	LL_ADC_REG_SetFlagEndOfConversion(ADC1, LL_ADC_REG_FLAG_EOC_UNITARY_CONV);
}


void AdcClock(void)
{
	switch (TwistedLeftState)
	{
	case TWISTED_LEFT_ST_FALSE:
		if (abs(KnobAngle(KNOB_LEFT) - LeftStartPos) >= TWIST_ANGLE_TH_TRUE)
		{
			IsTwistedLeft = TRUE;
			TwistedLeftState = TWISTED_LEFT_ST_TRUE;
		}
		break;

	case TWISTED_LEFT_ST_TRUE:
		if (abs(KnobAngle(KNOB_LEFT) - LeftStartPos) <= TWIST_ANGLE_TH_FALSE)
		{
			IsTwistedLeft = FALSE;
			TwistedLeftState = TWISTED_LEFT_ST_FALSE;
		}
		break;
	}

	switch (TwistedRightState)
	{
	case TWISTED_RIGHT_ST_FALSE:
		if (abs(KnobAngle(KNOB_RIGHT) - RightStartPos) >= TWIST_ANGLE_TH_TRUE)
		{
			IsTwistedRight = TRUE;
			TwistedRightState = TWISTED_RIGHT_ST_TRUE;
		}
		break;

	case TWISTED_RIGHT_ST_TRUE:
		if (abs(KnobAngle(KNOB_RIGHT) - RightStartPos) <= TWIST_ANGLE_TH_FALSE)
		{
			IsTwistedRight = FALSE;
			TwistedRightState = TWISTED_RIGHT_ST_FALSE;
		}
		break;
	}
}


Bool AdcIsTwistedLeft(void)
{
	return IsTwistedLeft;
}


void AdcClearIsTwistedLeft(void)
{
	IsTwistedLeft = FALSE;
	LeftStartPos = KnobAngle(KNOB_LEFT);
	TwistedLeftState = TWISTED_LEFT_ST_FALSE;
}


Bool AdcIsTwistedRight(void)
{
	return IsTwistedRight;
}


void AdcClearIsTwistedRight(void)
{
	IsTwistedRight = FALSE;
	RightStartPos = KnobAngle(KNOB_RIGHT);
	TwistedRightState = TWISTED_RIGHT_ST_FALSE;
}


static int KnobAngle(int Knob)
{
	int	AdcReading;

	// Set the ADC channel
	LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, Knob);

	// Enable the ADC, wait for it to be ready
	LL_ADC_Enable(ADC1);
	while (!LL_ADC_IsActiveFlag_ADRDY(ADC1))
		;

	// Start the conversion, wait for it to complete
	LL_ADC_REG_StartConversionSWStart(ADC1);
	while (!LL_ADC_IsActiveFlag_EOCS(ADC1))
		;

	// Disable the ADC
	LL_ADC_Disable(ADC1);

	AdcReading = LL_ADC_REG_ReadConversionData12(ADC1) + READING_OFFSET;

	return ((AdcReading * (KNOB_ANGLE_MAX - KNOB_ANGLE_MIN)) / (READING_MAX - READING_MIN)) +
			KNOB_ANGLE_MIN;
}
