#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp


Actor::Actor(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth)
	: GraphObject(imageID, startX, startY, dir, size, depth)
{}

TunnelMan::TunnelMan(StudentWorld* gameWorld) : Actor(TID_PLAYER, 30, 60, right, 1.0, 0)
{
	setVisible(true);
	settingWorld(gameWorld);
}
void TunnelMan::doSomething()
{
	//if (!getAlive()) // if dead, return immediately 
		//return;

	
	int ch;
	if (getWorld()->getKey(ch) == true)
	{
		Direction currentDirection = getDirection();
		int x = getX();
		int y = getY();
		// user hit a key this tick!
		switch (ch)
		{
		case KEY_PRESS_LEFT:
			//... face/move player to the left ...;
			if (currentDirection == left && x > 0) // if tunnelMan can move left and not out of bounds
			{
				moveTo(x - 1, y);
			}
			else // otherwise change direction
			{
				moveTo(x, y);
				setDirection(left);
				break;
			}
			
		case KEY_PRESS_RIGHT:
			//... face/move player to the right ...;
			if (currentDirection == right && x < VIEW_WIDTH-4) // if tunnelMan can move right and not out of bounds
			{
				moveTo(x + 1, y);
			}
			else // otherwise change direction
			{
				moveTo(x, y);
				setDirection(right);
				break;
			}
			
		case KEY_PRESS_UP:
			//... face/move player up ...;
			if (currentDirection == up && x < VIEW_HEIGHT - 4) // if tunnelMan can move up and not out of bounds
			{
				moveTo(x, y+1);
			}
			else // otherwise change direction
			{
				moveTo(x, y);
				setDirection(up);
				break;
			}

		case KEY_PRESS_DOWN:
			//... face/move player down ...;
			if (currentDirection == up && x < VIEW_HEIGHT - 4) // if tunnelMan can move down and not out of bounds
			{
				moveTo(x, y - 1);
			}
			else // otherwise change direction
			{
				moveTo(x, y);
				setDirection(down);
				break;
			}

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

	getWorld()->diggingEarth();

}