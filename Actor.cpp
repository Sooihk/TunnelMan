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


Human::Human(StudentWorld* w, int imageNum, int xCoords, int yCoords, Direction move, int health):Actor(w, imageNum, xCoords, yCoords, move, 1.0, 0)
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

void TunnelMan::addGoodies(int idNum)
{
	if (idNum == TID_GOLD)
	{
		gold++;
		getWorld()->increaseScore(10);
	}
	else if (idNum == TID_SONAR)
	{
		sonar++;
		getWorld()->increaseScore(75);
	}
	else if (idNum == TID_WATER_POOL)
	{
		water += 5;
		getWorld()->increaseScore(100);
	}
}

// .............................. PROTESTER CLASS ..............................

// base protester constructor
Protester::Protester(StudentWorld* gameWorld, int imageNum, int health) : Human(gameWorld, imageNum, 60, 60, left, health),
toLeave(false), ticksSincePreviousTurn(200), ticksTillYell(15)
{
	ticksToWaitBetweenMoves = max(0, 3 - (int)getWorld()->getLevel() / 4); // compute number of resting ticks
	randomNumberMoves();
}

void Protester::doSomething()
{
	// #1. check to see if protester is currently alive, if not return
	if (!isAlive)
	{
		return;
	}
	// #2 check if protester is in rest state during current tick
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
	// #3 if protester is in a leave-the-oil-field state as hit points reached zero
	if (toLeave)
	{
		// if protester is at exit point
		if (getX() == 60 && getY() == 60)
		{
			isDead(); // set bool variable to false
			getWorld()->decreaseProtester(); // decrease number of active protesters
			return;
		}
		// else protester must move one square close to exit point
		else
		{

		}
	}

}

// function generating random number of squares to move in current direction
void Protestor::randomNumberMoves()
{
	numofSquaresToMoveInCurrentDirection = rand() % (61 - 8) + 8; // random number between 8 and 60 inclusive 
}

// bool function returning true/false if protestor is facing in the direction of tunnelman
bool Protestor::protestorFacingPlayer()
{
	switch (getDirection()) // get direction protestor is facing
	{
	case up:
		if (getY() <= getWorld()->getPlayer()->getY()) // if protestor is facing player and above player
		{
			return true;
		}
	case right:
		if (getX() <= getWorld()->getPlayer()->getX()) // if protestor is facing player and right of player
		{
			return true;
		}
	case down:
		if (getY() >= getWorld()->getPlayer()->getY()) // if protestor is facing player and belwo player
		{
			return true;
		}
	case left:
		if (getY() >= getWorld()->getPlayer()->getY()) // if protestor is facing player and left of player
		{
			return true;
		}
	case none:
		return false;
	}
	return false;
}

void Protester::bribing()
{
	getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
	if (getID() == TID_PROTESTER) // if we're working with a regular protester,
	{
		getWorld()->increaseScore(25); // increases the player score by 25
		toLeave = true; // make them leave
	}
	else // else this is a hardcore protester
	{
		getWorld()->increaseScore(50); // increases the player score by 50
		getStunned(); // protester gets stunned and sets the amount of ticks they have to wait
	}
}

void Protester::getStunned()
{
	int lvl = getWorld()->getLevel();
	ticksToWaitBetweenMoves = max(50, 100 - (lvl * 10)); // function of the amount of ticks they have to wait
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
	if (!isAlive())
		return;
	if (stable) // if not alive, then return immediately 
	{
		if (getWorld()->earthAbove(getX(), getY() - 1)) // if there is earth below boulder
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
		if (getWorld()->earthAbove(getX(), getY() - 1) || getWorld()->checkBoulder(getX(), getY() - 4, 0))
			isDead(); // leaves the oil field 
		else //otherwise, move down
			moveTowards(getX(), getY() - 1);
		annoyPerson(); // we will also be checking if there is a person under to "annoy"
	}
}

void Boulder::annoyPerson()
{
	if (getWorld()->playerInRadius(this, 3)) // if a player is within radius,
		getWorld()->getPlayer()->actorAnnoyed(100); // we increase by 100
	Protester* p = getWorld()->protesterInRadius(this, 3); // grabs protester within radius
	if (p != nullptr) // if a protester is within radius, 
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
	if (!isAlive()) // if the squirt is dead, 
		return; // return immediately
	if (hitProtesters() || travel == 4) // if a protester is within 3 units and is in the squirt travel distance,
	{
		isDead(); // the squirt is put into a dead state
		return;
	}

	switch (getDirection())
	{
	case up:
		if (checkForObject(getX(), getY() + 1)) // checks for an earth or boulder directly above
		{
			isDead(); // squirt is put into a dead state
			return;
		}
		else
			moveTowards(getX(), getY() + 1)); // otherwise, move above one unit
		break;
	case down:
		if (checkForObject(getX(), getY() - 1)) // checks for an earth or boulder directly below
		{
			isDead(); // squirt is put into a dead state
			return;
		}
		else
			moveTowards(getX(), getY() - 1)); // otherwise, move below one unit
		break;
	case left:
		if (checkForObject(getX() - 1, getY()) // checks for an earth or boulder to the left
		{
			isDead(); // squirt is put into a dead state
			return;
		}
		else
			moveTowards(getX() - 1, getY())); // otherwise, move to the left one unit
		break;
	case right:
		if (checkForObject(getX() + 1, getY()) // checks for an earth or boulder to the right
		{
			isDead(); // squirt is put into a dead state
			return;
		}
		else
			moveTowards(getX() + 1, getY())); // otherwise, move to the right one unit
		break;
	case none:
		return;
	}

	travel++; // the squirt travel distance is incremented 
}

bool Squirt::checkForObject(int x, int y)
{
	return (getWorld()->checkEarth(x, y) || getWorld()->checkBoulder(x, y); // checks for earth or a boulder to the respected x and y coords
}

bool Squirt::hitProtesters()
{
	Protester* p = getWorld()->protesterInRadius(this, 3); // grabs a protestor within 3 units
	if (p != nullptr) // if it is a real protesters, 
	{
		p->actorAnnoyed(2); // annoy for two points
		return true;
	}
	else if (p == nullptr) // otherwise if there wasn't a protester
		return false;
}

// .............................. GOODIES CLASS ..............................

Goodies::Goodies(StudentWorld* w, int imageNum, int xCoords, int yCoords)
	: Actor(w, imageNum, xCoords, yCoords, right, 1.0, 2)
{
	setVisible(true);
	tick = 0;
}

void Goodies::disappear(int t) 
{
	if (tick == t) // checks if the time is up; 
		isDead(); // if so, the goodie will disappear
	else
		tick++; // else, just increment the ticks
}


// .............................. [BARREL] OIL CLASS ..............................

Oil::Oil(StudentWorld* w, int xCoords, int yCoords)
	: Goodies(w, TID_BARREL, xCoords, yCoords)
{
	setVisible(false);
}

void Oil::doSomething()
{
	if (!isAlive()) // if the oil is not currently alive,
		return; // return immediately
	if (!isVisible() && getWorld()->playerInRadius(this, 4)) // if not currently visible and tunnelMan is 4 away
	{
		setVisible(true); // make the barrel visible
		return;
	}
	if (getWorld()->playerInRadius(this, 3)) // if barrel is 3 units away from tunnelMan, (and visible)
	{
		isDead(); // the barrel "activates" and the state is now dead
		getWorld()->playSound(SOUND_FOUND_OIL); 
		getWorld()->increaseScore(1000); // increases player's score by 1000
		getWorld()->decreaseBarrel(); // barrels left is decreased; once all are picked up, level is complete
		return;
	}
}

// .............................. GOLD CLASS ..............................

Gold::Gold(StudentWorld* w, int xCoords, int yCoords, bool visible, bool dropped)
	: Goodies(w, TID_GOLD, xCoords, yCoords)
{
	setVisible(visible);
	goldDropped = dropped;
};

void Gold::doSomething()
{
	if (!isAlive()) // if the gold is not currently alive,
		return; // return immediately
	if (!isVisible() && getWorld()->playerInRadius(this, 4)) // if the gold isn't visible and tunnelman is within 4 units
	{
		setVisible(true); // make the gold visible
		return;
	}
	if (!goldDropped && getWorld()->playerInRadius(this, 3)) // if the gold is pickupable by tunnelMan and within 3 units of a tunnelman
	{
		int goodiesID = getID();
		isDead(); // state is set to dead
		getWorld()->getSound(SOUND_GOT_GOODIE); 
		getWorld()->getPlayer()->addGoodies(goodiesID); // increase the gold by 1 and player score by 10
		return;
	}
	if (goldDropped) // if gold is pickupable by protesters and within 3 units from a protester
	{
		Protester* p = (getWorld()->protesterInRadius(this, 3)); // grab a protester within radius of 3
		if (p != nullptr) // as long as it is a real protester,
		{
			isDead(); // set the gold to a dead state
			p->bribing(); // either stuns or gets a protester to leave
		}
		disappear(100); // sets the gold to disappear in 100 ticks
	}
}

// .............................. SONAR CLASS ..............................

Sonar::Sonar(StudentWorld* w, int xCoords, int yCoords)
	: Goodies(w, TID_SONAR, xCoords, yCoords)
{
	setVisible(true);
};

void Sonar::doSomething()
{
	if (!isAlive()) // if the sonar is not currently alive,
		return; // return immediately
	if (getWorld()->playerInRadius(this, 3)) // if the sonar is within 3 units from tunnelman,
	{
		int goodiesID = getID();
		isDead(); // state is set to dead
		getWorld()->getSound(SOUND_GOT_GOODIE);
		getWorld()->getPlayer()->addGoodies(goodiesID); // increase the sonar by 1 and player score by 75
		return;
	}
	int lvl = getWorld()->getLevel();
	int T = max(100, 300 - (10 * lvl));
	disappear(T); // sonar is always in temp state so it will be removed at the end of the tick
}

// .............................. WATER CLASS ..............................

Water::Water(StudentWorld* w, int xCoords, int yCoords)
	: Goodies(w, TID_WATER_POOL, xCoords, yCoords)
{
	setVisible(true);
}

void Water::doSomething()
{
	if (!isAlive()) // if the water is not currently alive,
		return; // return immediately
	if (getWorld()->playerInRadius(this, 3)) // if the water is within 3 units from tunnelman,
	{
		int goodiesID = getID();
		isDead(); // state is set to dead
		getWorld()->getSound(SOUND_GOT_GOODIE);
		getWorld()->getPlayer()->addGoodies(goodiesID); // increase the water by 5 and player score by 100
		return;
	}
	int lvl = getWorld()->getLevel();
	int T = max(100, 300 - (10 * lvl));
	disappear(T); // water is always in temp state so it will be removed at the end of the tick
}
