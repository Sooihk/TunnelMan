#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp


void TunnelMan::doSomething()
{
	if (!getAlive()) // if dead, return immediately 
		return;

	int ch;
	if (getWorld()->getKey(ch) == true)
	{
		// user hit a key this tick!
		switch (ch)
		{
		case KEY_PRESS_LEFT:
			//... face/move player to the left ...;
			break;
		case KEY_PRESS_RIGHT:
			//... face/move player to the right ...;
			break;
		case KEY_PRESS_UP:
			//... face/move player up ...;
			break;
		case KEY_PRESS_DOWN:
			//... face/move player down ...;
			break;
		case KEY_PRESS_SPACE:
			//... add a Squirt in front of the player...;
			break;
		case KEY_PRESS_ESCAPE:
			//... aborts program ...;
			break;
		}
	}
	//else occupied
		//remove and destroy the Earth objects from 4x4 area occupied from TunnelMan (to x+3, y+3)
		//play SOUND_DIG (look at StudentWorld section for help)

}