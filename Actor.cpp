#include "Actor.h"
#include "StudentWorld.h"
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

// .............................. ACTOR CLASS ..............................

// constructor
Actor::Actor(StudentWorld* w, int imageNum, int xCoords, int yCoords, Direction move,
	double size, unsigned int depth) :GraphObject(imageNum, xCoords, yCoords, move, size, depth), newWorld(w), actorAlive(true)
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

// .............................. HUMAN CLASS ..............................

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

// .............................. EARTH CLASS ..............................

Earth::Earth(StudentWorld* w, int xCoords, int yCoords) 
	: Actor(w, TID_EARTH, xCoords, yCoords, right, 0.25, 3)
{
	setVisible(true);
}

// .............................. TUNNELMAN CLASS ..............................

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

// .............................. PROTESTER CLASS ..............................

// base protestor constructor
Protester::Protester(StudentWorld* gameWorld) : Actor(gameWorld, TID_PROTESTER, 60, 60, left, 1.0, 0)
{
	setVisible(true);
	settingWorld(gameWorld);
}

void Protester::doSomething()
{

}

// .............................. BOULDER CLASS ..............................

Boulder::Boulder(StudentWorld* w, int xCoords, int yCoords)
	: Actor(w, TID_BOULDER, xCoords, yCoords, down, 1.0, 1)
{
	setVisible(true);
	stable = true;
	ticks = 0;
	falling = false;
}

void Boulder::doSomething()
{
	if (!isAlive()) // if not alive, then return immediately 
		return;
	if (stable)
	{
		if (getWorld()->aboveOrBelowEarth(getX(), getY() - 1)) // if there is earth below boulder
			return; // does nothing because it cant do anything w/ earth under
		else
			stable = false; // changes into "waiting" state for 30 ticks
	}
	if (ticks == 30) // the time period before a boulder drops
	{
		getWorld()->playSound(SOUND_FALLING_ROCK);
		falling = true; // the boulder is now in falling state
	}
	ticks++;
	if (falling) 
	{
		// if the boulder hits the bottom/Earth or runs into another boulder, the current boulder is dead
		if (getWorld()->aboveOrBelowEarth(getX(), getY() - 1) || getWorld()->checkBoulder(getX(), getY() - 4, 0))
			isDead();
		else //otherwise, move down
			moveTowards(getX(), getY() - 1); 
		annoyPerson(); // we will also be checking if there is a person under 
	}
}

void Boulder::annoyPerson()
{
	if (getWorld()->playerInRadius(this, 3)) // if a player is within radius,
		getWorld()->getPlayer()->actorAnnoyed(100); // we increase by 100
	Protester* p = getWorld()->protesterInRadius(this, 3); // grabs protestor within radius
	if (p != nullptr) // if a protestor is within radius, 
		p->actorAnnoyed(100); // we also increase by 100
}

// .............................. SQUIRT CLASS ..............................

Squirt::Squirt(StudentWorld* w, int xCoords, int yCoords, Direction dir)
	: Actor(w, TID_WATER_SPURT, xCoords, yCoords, dir, 1.0, 1)
{
	setVisible(true);
	travel = 0;
}

void Squirt::doSomething()
{
	if (!isAlive())
		return;

}

bool Squirt::hitProtestors()
{
	Protester* p = getWorld()->protesterInRadius(this, 3);
	if (p == nullptr)
		return false;
	else
	{

	}
}

// .............................. GOODIES CLASS ..............................

Goodies::Goodies(StudentWorld* w, int imageNum, int xCoords, int yCoords)
	: Actor(w, imageNum, xCoords, yCoords, right, 1.0, 2)
{
	setVisible(true);
	pickupAble = true;
	permanent = false;
}


// .............................. OIL CLASS ..............................

Oil::Oil(StudentWorld* w, int xCoords, int yCoords)
	: Goodies(w, TID_BARREL, xCoords, yCoords)
{
	setVisible(false);
}

void Oil::doSomething()
{
}

// .............................. GOLD CLASS ..............................

Gold::Gold(StudentWorld* w, int xCoords, int yCoords)
	: Goodies(w, TID_GOLD, xCoords, yCoords)
{
	// setVisible() is dependent on the code:
	// burried inside the Earth = invisible || dropepd by the tunnelman = visible

	//pickupAble = true; DEPENDS ON THE CODE, either the protesters or tunnelman can, never both

	//permanent = true; DEPENDS ON THE CODE, true = remain in the oil field, false = temp
};

void Gold::doSomething()
{
}

// .............................. SONAR CLASS ..............................

Sonar::Sonar(StudentWorld* w, int xCoords, int yCoords)
	: Goodies(w, TID_SONAR, xCoords, yCoords)
{
	setVisible(true);
	pickupAble = true; // only TunnelMan can pickup
	permanent = false; // temporary for T = max(100, 300 – 10*current_level_number)
};

void Sonar::doSomething()
{
}

// .............................. WATER CLASS ..............................

Water::Water(StudentWorld* w, int xCoords, int yCoords)
	: Goodies(w, TID_WATER_POOL, xCoords, yCoords)
{
	setVisible(true);
	pickupAble = true; // only TunnelMan can pickup
	permanent = false; // temporary for T = max(100, 300 – 10*current_level_number)
}

void Water::doSomething()
{
}