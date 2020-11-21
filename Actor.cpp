#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp


void TunnelMan::doSomething()
{
	if (!getAlive()) // if dead, return immediately 
		return;

	int key;
	if (getWorld()->getKey(key)) { // check if occupied (4X4)
		//remove and destroy the Earth objects from 4x4 area occupied from TunnelMan (to x+3, y+3)
		//play SOUND_DIG (look at StudentWorld section for help)
	}
	else{
		//check to see if a key was pressed, and act occordingly 
		// excape = abort
		// spacebar = fire squirt into oil field (if sufficient)
		// left/right/up/down = either face left/right/up/down OR move in that direction
	}

}