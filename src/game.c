//
// COMP-GENG 421 - Edgar Ramirez-Villa
//
// Game module
//

#include "main.h"
#include "accel.h"
#include "adc.h"
#include "game.h"
#include "lcd.h"
#include "nav.h"
#include <stdlib.h>


#define NAV_SEL				0x0001
#define NAV_UP				0x0002
#define NAV_DOWN			0x0004
#define NAV_LEFT			0x0008
#define NAV_RIGHT			0x0010
#define TILT_UP				0x0020
#define TILT_DOWN			0x0040
#define TILT_LEFT			0x0080
#define TILT_RIGHT			0x0100
#define TWIST_RIGHT			0x0200
#define TWIST_LEFT			0x0400
#define ACCEL_SHAKE 		0x0800

#define NUM_ACTIONS			12		// number of actions

#define GAME_ST_INIT		0
#define GAME_ST_START		1
#define GAME_ST_PROMPT		2
#define GAME_ST_WAIT		3
#define GAME_ST_RESULTS		4
#define GAME_ST_PAUSE		5
#define GAME_ST_DELAY		6

#define RESULTS_TICKS		5000	// 5 seconds
#define GAME_TIME			30000	//30 seconds
#define PROMPT_DELAY		200		//.2 seconds

#define START_TIMER			0
#define END_TIMER			1
#define WAIT_TIMER			2

typedef struct _ActionType
{
	char *	PromptStr;
	int		ActionBitmask;
} ActionType;


static ActionType ActionArray[NUM_ACTIONS] =
{
		{"Press Select Button  ", NAV_SEL},
		{"Press Up Button      ", NAV_UP},
		{"Press Down Button    ", NAV_DOWN},
		{"Press Left Button    ", NAV_LEFT},
		{"Press Right Button   ", NAV_RIGHT},
		{"Tilt Device Up       ", TILT_UP},
		{"Tilt Device Down     ", TILT_DOWN},
		{"Tilt Device Left     ", TILT_LEFT},
		{"Tilt Device Right    ", TILT_RIGHT},
		{"Twist Left Knob	   ", TWIST_LEFT},
		{"Twist Right Knob 	   ", TWIST_RIGHT},
		{"Shake Device		   ", ACCEL_SHAKE}
};

static char * BlankLineStr   = "                     ";
static char * GameNameStr    = "Fraudulent BopIt Game";
static char * StartPromptStr = "Press Select to Begin";
static int TickCounter;

static int ReadActions(void);
static void ClearActions(void);

//random number generator
//generates a random number between 0 and 11
int GenerateRandomIdx(){
	static int randomIdx;
	static int prevRandomIdx;

	do {
		prevRandomIdx = randomIdx;
		do{
			randomIdx = (rand() + TickCounter) & 0xF;
		} while(randomIdx > 11);
	} while(prevRandomIdx == randomIdx);

	return randomIdx;
}

void GameClock(void)
{
	static int ActionIdx;
	static int ActionsCorrect;
	static int GameState = GAME_ST_INIT;
	static int GameTimer = GAME_TIME;
	static int TimerState = WAIT_TIMER;
	static int HighScore = 0;
	static int AlreadyStoppedTimer = 0;
	static int PromptDelay = GAME_ST_DELAY;

	//handles the timer for the time remaining
	switch(TimerState)
	{
	case END_TIMER:
		GameTimer = GAME_TIME;
		TimerState = WAIT_TIMER;
		GameState = GAME_ST_RESULTS;
		break;
	case START_TIMER:
		if(--GameTimer <= 0){
			TimerState = END_TIMER;
		}
		if(GameTimer % 1000 == 0){
		LcdPrintf(0,0,"%03d", (GameTimer/1000));
		}
		break;
	case WAIT_TIMER:
		break;
	}

	switch(GameState)
	{
	//initialize the game screen
	//reset necessary variables
	//clear all actions
	case GAME_ST_INIT:
		LcdPrintf(0, 0, GameNameStr);
		LcdPrintf(1, 0, BlankLineStr);
		LcdPrintf(2, 0, StartPromptStr);
		LcdPrintf(3, 0, BlankLineStr);

		ActionIdx = GenerateRandomIdx();
		ActionsCorrect = 0;
		ClearActions();
		GameState = GAME_ST_START;
		break;

	//if select is pressed, start the game
	case GAME_ST_START:
		if (NavSelectIsPressed())
		{
			GameState = GAME_ST_PROMPT;
			LcdPrintf(0,0, BlankLineStr);
			LcdPrintf(0, 17, "%03d", ActionsCorrect);
			TimerState = START_TIMER;
			AlreadyStoppedTimer = 0;
		}
		break;

	//display the prompt to be completed by the user
	case GAME_ST_PROMPT:
		LcdPrintf(2, 0, BlankLineStr);
		LcdPrintf(2, 0, "%s", ActionArray[ActionIdx].PromptStr);
		ClearActions();
		PromptDelay = PROMPT_DELAY;
		GameState = GAME_ST_WAIT;
		break;

	//wait for an action to occur and determine if it is the right one
	case GAME_ST_WAIT:
		if (!ReadActions())
		{
			break;
		}
		if (ReadActions() == ActionArray[ActionIdx].ActionBitmask)
		{
			ActionsCorrect++;
			LcdPrintf(0, 5, "                     ");
			LcdPrintf(0, 17, "%03d", ActionsCorrect);
			//keeps track of the high score and sets a new one if needed
			if(ActionsCorrect > HighScore){
				HighScore = ActionsCorrect;
			}
		}
		else
		{
			GameState = GAME_ST_RESULTS;
			break;
		}
		ActionIdx = GenerateRandomIdx();
		GameState = GAME_ST_DELAY;
		break;

	//end the game and display the results
	case GAME_ST_RESULTS:
		LcdPrintf(0, 0, "Your Score : %2d      ", ActionsCorrect);
		LcdPrintf(2, 0, "High Score : %2d      ", HighScore);
		TickCounter = RESULTS_TICKS;
		GameState = GAME_ST_PAUSE;
		//necessary to make sure the timer is only reset once
		//otherwise, the timer state machine takes too much time
		if(!AlreadyStoppedTimer){
			TimerState = END_TIMER;
			AlreadyStoppedTimer = 1;
		}
		else {
			TimerState = WAIT_TIMER;
		}
		break;

	//pause the game to display the results for 5 seconds
	case GAME_ST_PAUSE:
		if (--TickCounter <= 0)
		{
			GameState = GAME_ST_INIT;
		}
		break;
	//.2 second delay between prompts for a more natural feel
	case GAME_ST_DELAY:
		if (--PromptDelay <= 0)
		{
			GameState = GAME_ST_PROMPT;
		}
		break;
	}

}

//read all actions input
//returns 0 if no inputs are active
static int ReadActions(void)
{
	return (NavSelectIsPressed() ? NAV_SEL    : 0) |
		   (NavUpIsPressed()     ? NAV_UP     : 0) |
		   (NavDownIsPressed()   ? NAV_DOWN   : 0) |
		   (NavLeftIsPressed()   ? NAV_LEFT   : 0) |
		   (NavRightIsPressed()  ? NAV_RIGHT  : 0) |
		   (AccelIsTiltedUp()    ? TILT_UP    : 0) |
		   (AccelIsTiltedDown()  ? TILT_DOWN  : 0) |
		   (AccelIsTiltedLeft()  ? TILT_LEFT  : 0) |
		   (AccelIsTiltedRight() ? TILT_RIGHT : 0) |
		   (AdcIsTwistedLeft()	 ? TWIST_LEFT : 0) |
		   (AdcIsTwistedRight()  ? TWIST_RIGHT: 0) |
		   (AccelIsShaken()      ? ACCEL_SHAKE: 0);
}

//clear all inputs
static void ClearActions(void)
{
	NavClearSelectIsPressed();
	NavClearUpIsPressed();
	NavClearDownIsPressed();
	NavClearLeftIsPressed();
	NavClearRightIsPressed();
	AccelClearIsTiltedUp();
	AccelClearIsTiltedDown();
	AccelClearIsTiltedLeft();
	AccelClearIsTiltedRight();
	AdcClearIsTwistedLeft();
	AdcClearIsTwistedRight();
	AccelClearIsShaken();
}
