#include "Actor.h"
#include "StudentWorld.h"
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

// constructor
Actor::Actor(StudentWorld* w, int imageNum, int xCoords, int yCoords, Direction move,
	double size, unsigned int dept) :GraphObject(imageNum, xCoords, yCoords, move, size, depth), newWorld(w), actorAlive(true)
{
	setVisible(true);
}
bool Actor::isAlive()
{
	return actorAlive;
}
StudentWorld* Actor::getWorld()
{
	return newWorld;
}
Actor::~Actor()
{
	setVisible(false);
}
void Actor::isDead()
{
	actorAlive = false;
}
void Actor::moveTowards(int x, int y)
{
	if (x > 60)
	{
		x = 60;
	}
	if (y > 60)
	{
		y = 60;
	}
	if (x < 0)
	{
		x = 0;
	}
	if (y < 0)
	{
		y = 0;
	}
	GraphObject::moveTo(x, y);
}

Human::Human(StudentWorld* w, int imageNum, int xCoords, int yCoords, Direction move, int health)
	:Actor(w, imageNum, xCoords, yCoords, move, 1.0, 0), human_HP(health) {}

void Human::decreaseHealthPoints(int damage)
{
	human_HP -= damage;
}
int Human::getHealthPoints()
{
	return human_HP;
}

TunnelMan::TunnelMan(StudentWorld* gameWorld) : Actor(gameWorld, TID_PLAYER, 30, 60, right, 1.0, 0)
{
	setVisible(true);
	settingWorld(gameWorld);
}
void TunnelMan::doSomething()
{
	//if (!getAlive()) // if dead, return immediately 
		//return;


	int ch; // holds the character input
	if (getWorld()->getKey(ch) == true) // returns true if user hits a key for the current tick
	{
		Direction currentDirection = getDirection(); // grabs current direction
		int x = getX(); // grabs current x coords
		int y = getY(); // grabs current y coords
		// user hit a key this tick!
		switch (ch)
		{
		case KEY_PRESS_LEFT:
			//... face/move player to the left ...;
			if (currentDirection == left && x > 0) // if tunnelMan can move left and not out of bounds
			{
				moveTo(x - 1, y); // updates location of GraphObject
			}
			else // otherwise change direction
			{
				moveTo(x, y);
			}
			setDirection(left);
			break;

		case KEY_PRESS_RIGHT:
			//... face/move player to the right ...;
			if (currentDirection == right && x < VIEW_WIDTH - 4) // if tunnelMan can move right and not out of bounds
			{
				moveTo(x + 1, y); // updates location of GraphObject
			}
			else // otherwise change direction
			{
				moveTo(x, y);
			}
			setDirection(right);
			break;

		case KEY_PRESS_UP:
			//... face/move player up ...;
			if (currentDirection == up && y < VIEW_HEIGHT - 4) // if tunnelMan can move up and not out of bounds
			{
				moveTo(x, y + 1); // updates location of GraphObject
			}
			else // otherwise change direction
			{
				moveTo(x, y);
			}
			setDirection(up);
			break;

		case KEY_PRESS_DOWN:
			//... face/move player down ...;
			if (currentDirection == down && y > 0) // if tunnelMan can move down and not out of bounds
			{
				moveTo(x, y - 1); // updates location of GraphObject
			}
			else // otherwise change direction
			{
				moveTo(x, y);
			}
			setDirection(down);
			break;

		case KEY_PRESS_SPACE:
			//... add a Squirt in front of the player...;
			break;
		case KEY_PRESS_ESCAPE:
			//... aborts program ...;
			break;
		}
	}

	getWorld()->diggingEarth(); // StudentWorld function digs earth based on player movement

}

// base protestor constructor
Protestor::Protestor(StudentWorld* gameWorld) : Actor(gameWorld, TID_PROTESTER, 60, 60, left, 1.0, 0)
{
	setVisible(true);
	settingWorld(gameWorld);
}

void Protestor::doSomething()
{

}

void Protestor::set
