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
}

void Boulder::doSomething()
{
	if (!isAlive())
		return;
	if (stable)
	{
		if (getWorld()->aboveOrBelowEarth(getX(), getY() - 1)) // if there is earth below boulder
			return;
		else
			stable = false;
	}
	if (ticks == 30) // the time period before a boulder drops
	{
		getWorld()->playSound(SOUND_FALLING_ROCK);
		if (getWorld()->aboveOrBelowEarth(getX(), getY() - 1) || getWorld()->checkBoulder(getX(), getY() - 4, 0))
			isDead();
		else
			moveTowards(getX(), getY() - 1);
		annoyPerson();
	}
	ticks++;
}

void Boulder::annoyPerson()
{
	if (getWorld()->playerInRadius(this, 3))
		getWorld()->getPlayer()->actorAnnoyed(100);
	Protester* p = getWorld()->protesterInRadius(this, 3);
	if (p != nullptr)
		p->actorAnnoyed(100);
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
#include "Actor.h"
#include "StudentWorld.h"
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
// constructor


// .............................. ACTOR CLASS ..............................

Actor::Actor(StudentWorld* w, int imageNum, int xCoords, int yCoords, Direction move, double size, unsigned int depth) :GraphObject(imageNum, xCoords, yCoords, move, size, depth), newWorld(w), actorAlive(true)
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


Human::Human(StudentWorld* w, int imageNum, int xCoords, int yCoords, Direction move, int health) :Actor(w, imageNum, xCoords, yCoords, move, 1.0, 0)
{
	human_HP = health;
}

void Human::decreaseHealthPoints(int damage)
{
	human_HP -= damage;
}
int Human::getHealthPoints()
{
	return human_HP;
}

// .............................. EARTH CLASS ..............................


// Earth class constructor
Earth::Earth(StudentWorld* w, int xCoords, int yCoords) : Actor(w, TID_EARTH, xCoords, yCoords, right, 0.25, 3)
{
	setVisible(true);
}
void Earth::doSomething() {}


// .............................. TUNNELMAN CLASS ..............................

TunnelMan::TunnelMan(StudentWorld* gameWorld) : Human(gameWorld, TID_PLAYER, 30, 60, right, 10)
{
	water = 5;
	sonarCharge = 1;
	goldNuggets = 0;

}
void TunnelMan::doSomething()
{
	// #1
	if (!isAlive()) // check is tunnelman is alive
	{
		return;
	}
	// #2 if Tunnelman image overlaps any Earth objects, remove earth objects and make digging sound
	if (getWorld()->diggingEarth(getX(), getY())) // call digging fucntion to delete earth object
	{
		getWorld()->playSound(SOUND_DIG); // play digging sound
	}

	int userInput;
	// #3 Check to see if human player pressed key
	if (getWorld()->getKey(userInput) == true) // check to see if user pressed a key during this tick
	{
		switch (userInput)
		{
			// #3a If user pressed Escape key, abort the curret level and set tunnelman to be dead
		case KEY_PRESS_ESCAPE:
		{
			isDead();
			break;
		}
		// 3b If user pressed spacebar, tunnelman fire a squirt
		case KEY_PRESS_SPACE:
		{

		}
		// 3cd If user pressed any of the direction keys: (left,right,down,up)
		case KEY_PRESS_RIGHT:
		{
			moveTowardsDirection(right); // call function which moves TunnelMan towards the right
			break;
		}
		case KEY_PRESS_DOWN:
		{
			moveTowardsDirection(down); // call function which moves TunnelMan down
			break;
		}
		case KEY_PRESS_UP:
		{
			moveTowardsDirection(up); // call function which moves TunnelMan up
			break;
		}
		case KEY_PRESS_LEFT:
		{
			moveTowardsDirection(left); // call function which moves TunnelMan towards the left
			break;
		}

		// ADD OTHER CASES, SONAR GOLD ....

		}

	}


}

// function which moves tunnelman towards player's chosen key direction
void TunnelMan::moveTowardsDirection(Direction dir)
{
	switch (dir) {
	case up: // case where player chose up movement key
	{
		if (getDirection() == up) // if TunnelMan was already facing up direction
		{
			if (!getWorld()->checkBoulder(getX(), getY() + 1)) // check if boulder above tunnelman
				moveTowards(getX(), getY() + 1); // then move one block up
			else moveTowards(getX(), getY()); // else stay put
		}
		else setDirection(up); // else set TunnelMan direction to up
		break;
	}
	case down: // case where player chose down movement key
	{
		if (getDirection() == down) // if TunnelMan was already facing up direction
		{
			if (!getWorld()->checkBoulder(getX(), getY() - 1)) // check if boulder below tunnelman
				moveTowards(getX(), getY() - 1); // then move one block down
			else moveTowards(getX(), getY()); // else stay put
		}
		else setDirection(down); // else set TunnelMan direction to down
		break;
	}
	case left: // case where player chose left movement key
	{
		if (getDirection() == left) // if TunnelMan was already facing up direction
		{
			if (!getWorld()->checkBoulder(getX() - 1, getY())) // check if boulder left of tunnelman
				moveTowards(getX() - 1, getY()); // then move one block left
			else moveTowards(getX(), getY()); // else stay put
		}
		else setDirection(left); // else set TunnelMan direction to left
		break;
	}
	case right: // case where player chose right movement key
	{
		if (getDirection() == right) // if TunnelMan was already facing up direction
		{
			if (!getWorld()->checkBoulder(getX() + 1, getY())) // check if boulder right of tunnelman
				moveTowards(getX() + 1, getY()); // then move one block right
			else moveTowards(getX(), getY()); // else stay put
		}
		else setDirection(right); // else set TunnelMan direction to right
		break;
	}

	case none: return;
	}
}

// function which decreases TunnelMan's health points, and declares dead if hp is <= 0
void TunnelMan::actorAnnoyed(int damage)
{
	decreaseHealthPoints(damage); // call function to decrease health points
	if (getHealthPoints() <= 0) // if tunnelman's health is <= 0
	{
		isDead(); // set his bool status to false, dead
		getWorld()->playSound(SOUND_PLAYER_GIVE_UP); // play give up sound
	}
}

// .............................. PROTESTER CLASS ..............................

// base protestor constructor
Protestor::Protestor(StudentWorld* gameWorld, int imageNum, int health) : Human(gameWorld, imageNum, 60, 60, left, health),
toLeave(false), ticksSincePreviousTurn(200), ticksTillYell(15)
{
	ticksToWaitBetweenMoves = max(0, 3 - (int)getWorld()->getLevel() / 4); // compute number of resting ticks
	randomNumberMoves();
}

void Protestor::doSomething()
{
	// #1. check to see if protestor is currently alive, if not return
	if (!isAlive)
	{
		return;
	}
	// #2 check if protestor is in rest state during current tick
	if (ticksToWaitBetweenMoves > 0)
	{
		ticksToWaitBetweenMoves--;
		return;
	}
	else
	{
		ticksToWaitBetweenMoves = max(0, 3 - (int)getWorld()->getLevel() / 4); // compute number of resting ticks
		ticksTillYell++;
		ticksSincePreviousTurn++;
	}
	// #3 if protestor is in a leave-the-oil-field state as hit points reached zero
	if (toLeave)
	{
		// if protestor is at exit point
		if (getX() == 60 && getY() == 60)
		{
			isDead(); // set bool variable to false
			getWorld()->decreaseProtestor(); // decrease number of active protestors
			return;
		}
		// else protestor must move one square close to exit point
		else
		{

		}
	}

}

void Protestor::randomNumberMoves()
{
	numofSquaresMove = rand() % (61 - 8) + 8; // random number between 8 and 60 inclusive 
}

// .............................. BOULDER CLASS ..............................

Boulder::Boulder(StudentWorld* w, int xCoords, int yCoords)
	: Actor(w, TID_BOULDER, xCoords, yCoords, down, 1.0, 1)
{
	setVisible(true);
	stable = true;
	ticks = 0;
}

void Boulder::doSomething()
{
	if (!isAlive())
		return;
	if (stable)
	{
		if (getWorld()->earthAbove(getX(), getY() - 1)) // if there is earth below boulder
			return;
		else
			stable = false;
	}
	if (ticks == 30) // the time period before a boulder drops
	{
		getWorld()->playSound(SOUND_FALLING_ROCK);
		if (getWorld()->earthAbove(getX(), getY() - 1) || getWorld()->checkBoulder(getX(), getY() - 4, 0))
			isDead();
		else
			moveTowards(getX(), getY() - 1);
		annoyPerson();
	}
	ticks++;
}

void Boulder::annoyPerson()
{
	if (getWorld()->playerInRadius(this, 3))
		getWorld()->getPlayer()->actorAnnoyed(100);
	Protester* p = getWorld()->protesterInRadius(this, 3);
	if (p != nullptr)
		p->actorAnnoyed(100);
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