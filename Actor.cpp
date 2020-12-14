#include "Actor.h"
#include "StudentWorld.h"
#include "GraphObject.h"
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
			if (water > 0) // check to see if tunnelman has enough water charges
			{
				water--;
				shootWater();
			}
			break;
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
		case 'z':
		case 'Z': // using a sonar kit
		{
			if (sonarCharge > 0) // as long as they have one or more sonar charges,
			{
				sonarCharge--;
				getWorld()->checkGoodies(getX(), getY(), 12); // check for oil/gold within a radius of 12 units
			}
			break;
		}
		case KEY_PRESS_TAB: // bribbing with gold
		{
			if (goldNuggets > 0) // as long as they have one or more gold, 
			{
				goldNuggets--;
				getWorld()->actorAdded(new Gold(getWorld(), getX(), getY(), true, true)); // a new gold will be put into the oil field
			}
			break;
		}
		}

	}
}
// shoots squirt object in direction of tunnelman
void TunnelMan::shootWater()
{
	switch (getDirection())
	{
	case up:
	{
		shootWaterAux(getX(), getY() + 4);
		break;
	}
	case down:
	{
		shootWaterAux(getX(), getY() - 4);
		break;
	}
	case left:
	{
		shootWaterAux(getX() - 4, getY());
		break;
	}
	case right:
	{
		shootWaterAux(getX() + 4, getY());
		break;
	}
	case none:
	{
		return;
	}
	}
	getWorld()->playSound(SOUND_PLAYER_SQUIRT);
}

// function checking if there is no earth or boulder objects in front of tunnelman, if so then spawn a squirt object
void TunnelMan::shootWaterAux(int x, int y)
{
	// if theres no earth or boulder within the 4 units of whichever direction,
	if (!getWorld()->checkEarth(x, y) && !getWorld()->checkBoulder(x, y))
	{
		// we will add a new squirt obj into the oil field at the 4 spaces in front
		getWorld()->actorAdded(new Squirt(getWorld(), x, y, getDirection()));
	}

}

// add items to TunnelMan's inventory and update his score
void TunnelMan::addGoodies(int numID)
{
	if (numID == TID_GOLD)
	{
		goldNuggets++;
		getWorld()->increaseScore(10); // gold increases score by 10
	}
	else if (numID == TID_SONAR)
	{
		sonarCharge++;
		getWorld()->increaseScore(75); // sonar increases score by 75
	}
	else if (numID == TID_WATER_POOL)
	{
		water += 5;
		getWorld()->increaseScore(100); // water increases score by 100
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
			{
				moveTowards(getX(), getY() + 1); // then move one block up
			}
			else
			{
				moveTowards(getX(), getY()); // else stay put
			}
		}
		else
		{
			setDirection(up); // else set TunnelMan direction to up
		}
		break;
	}
	case down: // case where player chose down movement key
	{
		if (getDirection() == down) // if TunnelMan was already facing up direction
		{
			if (!getWorld()->checkBoulder(getX(), getY() - 1)) // check if boulder below tunnelman
			{
				moveTowards(getX(), getY() - 1); // then move one block down
			}
			else
			{
				moveTowards(getX(), getY()); // else stay put
			}
		}
		else
		{
			setDirection(down); // else set TunnelMan direction to down
		}
		break;
	}
	case left: // case where player chose left movement key
	{
		if (getDirection() == left) // if TunnelMan was already facing up direction
		{
			if (!getWorld()->checkBoulder(getX() - 1, getY())) // check if boulder left of tunnelman
			{
				moveTowards(getX() - 1, getY()); // then move one block left
			}
			else
			{
				moveTowards(getX(), getY()); // else stay put
			}
		}
		else
		{
			setDirection(left); // else set TunnelMan direction to left
		}
		break;
	}
	case right: // case where player chose right movement key
	{
		if (getDirection() == right) // if TunnelMan was already facing up direction
		{
			if (!getWorld()->checkBoulder(getX() + 1, getY())) // check if boulder right of tunnelman
			{
				moveTowards(getX() + 1, getY()); // then move one block right
			}
			else
			{
				moveTowards(getX(), getY()); // else stay put
			}
		}
		else
		{
			setDirection(right); // else set TunnelMan direction to right
		}
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
Protestor::Protestor(StudentWorld* gameWorld, int imageNum, int health) : Human(gameWorld, imageNum, 60, 60, left, health)
{
	toLeave = false;
	ticksSincePreviousTurn = 200;
	ticksTillYell = 15;
	randomNumberMoves();
	ticksToWaitBetweenMoves = max(0, 3 - (int)getWorld()->getLevel() / 4); // compute number of resting ticks
}

void Protestor::doSomething()
{
	// #1. check to see if protestor is currently alive, if not return
	if (!isAlive())
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
		// 3a if protestor is at exit point
		if (getX() == 60 && getY() == 60)
		{
			isDead(); // set bool variable to false
			getWorld()->decreaseProtestor(); // decrease number of active protestors
			return;
		}
		getWorld()->movingtoExitPoint(this); // // 3b else protestor must move one square close to exit point, move on sqaure close to the exit point
		return;
	}
	// #4 Check to see if protestor is within 4 units of TunnelMan and shout
	if (getWorld()->playerInRadius(this, 4) && protestorFacingTunnelMan()) // if protestor is within 4 units of Tunnelman and facing in Tunnelman direction
	{
		// check protestor hasn't shouted in last non resting 15 ticks
		if (ticksTillYell > 15)
		{
			getWorld()->playSound(SOUND_PROTESTER_YELL); // 4a. play shout sound
			getWorld()->getPlayer()->actorAnnoyed(2); //4b.  tunnelman is annoyed by 2 points
			ticksTillYell = 0; // 4c. update protestor shouting resting state
			return;
		}
		return;
	}
	// #5 for Hard_Core_Protestor
	if (getID() == TID_HARD_CORE_PROTESTER)
	{
		int M = 16 + int(getWorld()->getLevel()); // 5a. compute value M 
		// 5b. If Hard_Core_Protestor is within M horizontal or vertical from TunnelMan, then protestor can receive
		// TunnelMan cellphone signal and moves towards his direction
		Direction receiver = getWorld()->cellphoneSignalDirection(this, M); // 5bi. Determine which horizontal/vertical direction to move
		if (receiver != none)
		{
			moveTowardsDirection(receiver); // 5bii,iii. Change protestor's direction to face this new direction and move one square in this direction
			return;
		}
	}
	// #5 If Protestor is in straight horizontal or vertical line of sight to the TunnelMan 
	Direction lineofSight = directionTowardsTunnelMan(); // to get direction where protestor would be facing tunnelman 
	// if protestor is more than 4 units away, in a straight horizontal/vertial line of sigh to the TunnelMan
	if (lineofSight != none && (!getWorld()->playerInRadius(this, 4)) && straightTowardsTunnelMan(lineofSight))
	{
		setDirection(lineofSight); // change direction of protestor in the direction of TunnelMan
		moveTowardsDirection(lineofSight); // take one step toward TunnelMan
		numofSquaresToMoveInCurrentDirection = 0; // set numSquaresToMoveInCurrentDirection to 0, forcing it to pick a new direction/distance
		// to move during its next non-resting tick. 
		return;
	}

	// #6 Otherwise protestor can't directly see TunnelMan 
	numofSquaresToMoveInCurrentDirection--; // decrement variable by 1
	if (numofSquaresToMoveInCurrentDirection <= 0) // if protestor has finished walking numSquares... steps in its currently selected direction
	{
		Direction newDirection = none;
		newDirection = randomNewDirection(); // protestor will pick random new direction to move
		while (true) // infinte while loop unless break
		{
			newDirection = randomNewDirection(); // protestor will pick random new direction to move
			if (getWorld()->canActorMoveThisDirection(getX(), getY(), newDirection)) // if random direction is not blocked by earth or boulders
			{
				break;
			}
		}
		setDirection(newDirection); // change protestor's direction to this new chosenn direction
		randomNumberMoves(); // protestor will then pick a new value for numSquaresToMoveInCurrentDirection that will govern
		// how far it should move in the selected direction
	}

	// #7 If protestor is at a T intersection where it can turn and move perpendicular and hasn't made
	// a perpendicular turn in 200 non resting ticks
	else if ((ticksSincePreviousTurn > 200) && protestoratIntersection())
	{
		pickDirectionToTurn(); // pick direction for protestor to turn
		ticksSincePreviousTurn = 0; // protestor made turn so wait at least 200 more non resting ticks
		randomNumberMoves(); // pick new value for numSquaresToMoveInCurrentDirection that govern how far it will move in selected perpendicular direction
		
	}

	// #8 Protestor will take one step it is currently facing
	moveTowardsDirection(getDirection());

	// #9 If protestor is blocked from taking a step in currently facing direction, set numSquaresToMoveInCurrentDirection to 0
	// which results in a new direction being chosen during the protestor's next non resting tick, protestor do nothing current tick
	if (!getWorld()->canActorMoveThisDirection(getX(), getY(), getDirection()))
	{
		numofSquaresToMoveInCurrentDirection = 0;
	}
}

// function which will return random direction for protestor to go
GraphObject::Direction Protestor::randomNewDirection()
{
	int randomNum = rand() % 4; // random nummber from 0-3 inclusive
	switch (randomNum)
	{
		case 0:
		{
			return up;
		}
		case 1:
		{
			return down;
		}
		case 2:
		{
			return left;
		}
		case 3:
		{
			return right;
		}
	}
	return none;
}
// function generating random number of squares to move in current direction
void Protestor::randomNumberMoves()
{
	// rand() format rand()%((userEnd - userBeg) + 1) + userBeg; 
	numofSquaresToMoveInCurrentDirection = rand() % (60 - 8 + 1) + 8; // random number between 8 and 60 inclusive 
}

// bool function returning true/false if protestor is facing in the direction of tunnelman
bool Protestor::protestorFacingTunnelMan()
{
	switch (getDirection()) // get direction protestor is facing
	{
		case up:
		{
			return (getY() <= getWorld()->getPlayer()->getY()); // if protestor is facing player and above player
		}	
		case right:
		{
			return (getX() <= getWorld()->getPlayer()->getX()); // if protestor is facing player and right of player
		}
		case down:
		{
			return (getY() >= getWorld()->getPlayer()->getY()); // if protestor is facing player and belwo player
		}
		case left:
		{
			return (getY() >= getWorld()->getPlayer()->getY()); // if protestor is facing player and left of player
		}
		case none:
		{
			return false;
		}
	}
	return false;
}

void Protestor::bribing()
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

void Protestor::getStunned()
{
	ticksToWaitBetweenMoves = max(50, 100 - ((int)getWorld()->getLevel() * 10)); // function of the amount of ticks they have to wait
}

//move in set direction
void Protestor::moveTowardsDirection(Direction dir)
{
	switch (dir)
	{
		case up:
		{
			if (getDirection() == up)
			{
				if (getY() == 60) // 60 is limit for human to reach in terms of rows
				{
					setDirection(down);
				}
				moveTowards(getX(), getY() + 1); // else move up 1
			}
			else
			{
				setDirection(up);
			}
			break;
		}
		case right:
		{
			if (getDirection() == right)
			{
				if (getX() == 60) // 60 is limit for human to reach in terms of columns 
				{
					setDirection(left);
				}
				moveTowards(getX() + 1, getY()); // else move right 1
			}
			else
			{
				setDirection(right);
			}
			break;
		}
		case down:
		{
			if (getDirection() == down)
			{
				if (getY() == 0) // 0 is limit for human to reach in terms of rows
				{
					setDirection(up);
				}
				moveTowards(getX(), getY() - 1); // else move down 1
			}
			else
			{
				setDirection(down);
			}
			break;
		}
		case left:
		{
			if (getDirection() == left)
			{
				if (getX() == 0) // 0 is limit for human to reach in terms of columns
				{
					setDirection(right);
				}
				moveTowards(getX() - 1, getY()); // else move left 1
			}
			else
			{
				setDirection(left);
			}
			break;
		}
		case none:
		{
			return;
		}
	}
}

// function which decreases protestor's health and sets to dead if hp = 0
void Protestor::actorAnnoyed(int damage)
{
	if (toLeave) // check if protestor's status is set to leave the oil field
	{
		return;
	}
	getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
	decreaseHealthPoints(damage); // decrease protestor's health by damage points
	getStunned();

	if (getHealthPoints() <= 0) // check to see if protestor still has enough health points to stay in the field
	{
		getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
		toLeave = true;
		ticksToWaitBetweenMoves = 0;
		if (damage == 100) // check to see if protestor was hit a boulder
		{
			getWorld()->increaseScore(500); // increase score by 500
		}
		else if (getID() == TID_PROTESTER) // else if protestor was a regular protestor
		{
			getWorld()->increaseScore(100); // increase score by 100
		}
		else // else protestor was a hardcore protestor
		{
			getWorld()->increaseScore(250); // increase score by 250
		}
	}

}
// function to return what direction protestor sees tunnelman
GraphObject::Direction Protestor::directionTowardsTunnelMan()
{
	int tunnelManYLocation = getWorld()->getPlayer()->getY();
	int tunnelManXLocation = getWorld()->getPlayer()->getX();

	if (getX() == tunnelManXLocation && getY() == tunnelManYLocation) // if protestor is on top of tunnelman
	{
		return getDirection();
	}
	if (getX() == tunnelManXLocation) //if tunnelman and protestor in same column
	{
		if (getY() > tunnelManYLocation) // if protestor is above the tunnelman
		{
			return down;
		}
		if (getY() < tunnelManYLocation) // if protestor is below the tunnelman
		{
			return up;
		}
	}
	if (getY() == tunnelManYLocation) //if tunnelman and protestor in same row
	{
		if (getX() > tunnelManXLocation) // if protestor is right of tunnelman
		{
			return left;
		}
		if (getX() < tunnelManXLocation) // if protestor is left of tunnelman
		{
			return right;
		}
	}
	
	return none;
}

// function to check if protestor direction's has a straight line of sight to TunnelMan
bool Protestor::straightTowardsTunnelMan(Direction dir)
{
	int tunnelManYLocation = getWorld()->getPlayer()->getY();
	int tunnelManXLocation = getWorld()->getPlayer()->getX();

	switch (dir)
	{
		case up:
		{
			for (int i = getY(); i <= tunnelManYLocation; i++) // for loop from protestor row value to tunnelman row value
			{
				if (getWorld()->checkBoulder(getX(), i) || getWorld()->checkEarth(getX(), i)) // if any earth or boulder in between both position, false
				{
					return false;
				}
			}
			return true; // return true, when there is a straight line of sight from protestor to tunnelman
			break;
		}
		case down:
		{
			for (int i = getY(); i >= tunnelManYLocation; i--) // for loop from protestor row value to tunnelman row value
			{
				if (getWorld()->checkBoulder(getX(), i) || getWorld()->checkEarth(getX(), i)) // if any earth or boulder in between both position, false
				{
					return false;
				}
			}
			return true; // return true, when there is a straight line of sight from protestor to tunnelman
			break;
		}
		case right:
		{
			for (int i = getX(); i <= tunnelManXLocation; i++) // for loop from protestor column value to tunnelman column value
			{
				if (getWorld()->checkBoulder(i, getY()) || getWorld()->checkEarth(i, getY())) // if any earth or boulder in between both position, false
				{
					return false;
				}
			}
			return true; // return true, when there is a straight line of sight from protestor to tunnelman
			break;
		}
		case left:
		{
			for (int i = getX(); i >= tunnelManXLocation; i--) // for loop from protestor column value to tunnelman column value
			{
				if (getWorld()->checkBoulder(i, getY()) || getWorld()->checkEarth(i, getY())) // if any earth or boulder in between both position, false
				{
					return false;
				}
			}
			return true; // return true, when there is a straight line of sight from protestor to tunnelman
			break;
		}
		case none:
		{
			return false;
		}
	}
	return false;
}

// bool fucntion to return true or false if protestor can move perpendicular 
bool Protestor::protestoratIntersection()
{
	// if protestor is facing right or left and can move perpendicular up or down
	if (getDirection() == right || getDirection() == left)
	{
		return (getWorld()->canActorMoveThisDirection(getX(), getY(), up) || getWorld()->canActorMoveThisDirection(getX(), getY(), down));
	}
	// if protestor is facing up or down and can move perpendicular right or left
	else
	{
		return (getWorld()->canActorMoveThisDirection(getX(), getY(), right) || getWorld()->canActorMoveThisDirection(getX(), getY(), left));
	}
}

// function directing which of the 2 perpendicular directions are viable
void Protestor::pickDirectionToTurn()
{
	// if protestor is facing right or left and can move perpendicular up or down
	if (getDirection() == right || getDirection() == left)
	{
		// if protestor can not move up at the intersection then set direction down
		if (!getWorld()->canActorMoveThisDirection(getX(), getY(), up))
		{
			setDirection(down);
		}
		// if protestor can not move down at the intersection then set direction up
		else if (!getWorld()->canActorMoveThisDirection(getX(), getY(), down))
		{
			setDirection(up);
		}
		// if both choices are viable then pick one of 2 choices randomly
		else
		{
			int x = rand() % 2;
			switch (x)
			{
				case 0:
				{
					setDirection(up);
				}
				case 1:
				{
					setDirection(down);
				}
			}
		}
	}
	// if protestor is facing up or down and can move perpendicular right or left
	else
	{
		// if protestor can not move left at the intersection then set direction right
		if (!getWorld()->canActorMoveThisDirection(getX(), getY(), left))
		{
			setDirection(right);
		}
		// if protestor can not move right at the intersection then set direction left
		else if (!getWorld()->canActorMoveThisDirection(getX(), getY(), right))
		{
			setDirection(left);
		}
		// if both choices are viable then pick one of 2 choices randomly
		else
		{
			int x = rand() % 2;
			switch (x)
			{
				case 0:
				{
					setDirection(left);
				}
				case 1:
				{
					setDirection(right);
				}
			}
		}
	}
}

// regular protestor constructor
Regular_Protestor::Regular_Protestor(StudentWorld* w) : Protestor(w, TID_PROTESTER, 5) {}

// hardcore protestor constructor
Hardcore_Protestor::Hardcore_Protestor(StudentWorld* w) : Protestor(w, TID_HARD_CORE_PROTESTER, 20) {}
// .............................. BOULDER CLASS ..............................

Boulder::Boulder(StudentWorld* w, int xCoords, int yCoords) // constructor
	: Actor(w, TID_BOULDER, xCoords, yCoords, down, 1.0, 1)
{
	setVisible(true);
	currentlyStable = true;
	ticks = 0;
	fallingDown = false;
	w->diggingEarth(xCoords, yCoords);
}

void Boulder::doSomething()
{
	if (!isAlive()) // check is boulder is currently alive
	{
		return;
	}
	if (currentlyStable) // if not alive, then return immediately 
	{
		if (getWorld()->earthAbove(getX(), getY() - 1)) // if there is earth below boulder
		{
			return; // does nothing because it cant do anything w/ earth under
		}
		else
		{
			currentlyStable = false; // changes into "waiting" state for 30 ticks
		}
	}
	if (ticks == 30) // the time period before a boulder drops
	{
		getWorld()->playSound(SOUND_FALLING_ROCK);
		fallingDown = true; // the boulder is now in falling state
	}
	ticks++;
	if (fallingDown)
	{
		// if the boulder hits the bottom/Earth or runs into another boulder, the current boulder is dead
		if (getWorld()->earthAbove(getX(), getY() - 1) || getY() == 0 || getWorld()->checkBoulder(getX(), getY() - 4, 0))
		{
			isDead(); // leaves the oil field 
		}
		else //otherwise, move down
		{
			moveTowards(getX(), getY() - 1);
		}
		annoyPerson(); // we will also be checking if there is a person under to "annoy"
	}
}

void Boulder::annoyPerson()
{
	if (getWorld()->playerInRadius(this, 3)) // if a player is within radius,
	{
		getWorld()->getPlayer()->actorAnnoyed(100); // we increase by 100
	}
	Protestor* p = getWorld()->protesterInRadius(this, 3); // grabs protester within radius
	if (p != nullptr) // if a protester is within radius, 
	{
		p->actorAnnoyed(100); // we also increase by 100
	}
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
	{
		return; // return immediately
	}
	if (hitProtesters() || travel == 4) // if a protester is within 3 units and is in the squirt travel distance,
	{
		isDead(); // the squirt is put into a dead state
		return;
	}

	switch (getDirection())
	{
		case up:
		{
			if (checkForObject(getX(), getY() + 1)) // checks for an earth or boulder directly above
			{
				isDead(); // squirt is put into a dead state
				return;
			}
			else
			{
				moveTowards(getX(), getY() + 1); // otherwise, move above one unit
			}
			break;
		}
		case down:
		{
			if (checkForObject(getX(), getY() - 1)) // checks for an earth or boulder directly below
			{
				isDead(); // squirt is put into a dead state
				return;
			}
			else
			{
				moveTowards(getX(), getY() - 1); // otherwise, move below one unit
			}
			break;
		}
		case left:
		{
			if (checkForObject(getX() - 1, getY())) // checks for an earth or boulder to the left
			{
				isDead(); // squirt is put into a dead state
				return;
			}
			else
			{
				moveTowards(getX() - 1, getY()); // otherwise, move to the left one unit
			}
			break;
		}
		case right:
		{
			if (checkForObject(getX() + 1, getY())) // checks for an earth or boulder to the right
			{
				isDead(); // squirt is put into a dead state
				return;
			}
			else
			{
				moveTowards(getX() + 1, getY()); // otherwise, move to the right one unit
			}
			break;
		}
		case none:
		{
			return;
		}
	}
	travel++; // the squirt travel distance is incremented 
}

// function to check if squirt doesn't hit earth or boulder object
bool Squirt::checkForObject(int x, int y)
{
	return (getWorld()->checkEarth(x, y) || getWorld()->checkBoulder(x, y)); // checks for earth or a boulder to the respected x and y coords
}

//// function which damages protestor 
bool Squirt::hitProtesters()
{
	Protestor* grabProtestor = getWorld()->protesterInRadius(this, 3); // grabs a protestor within 3 units
	if (grabProtestor != nullptr) // if it is a real protesters, 
	{
		grabProtestor->actorAnnoyed(2); // annoy for two points
		return true;
	}
	return false; // otherwise if there wasn't a protester
	
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
	{
		isDead(); // if so, the goodie will disappear
	}
	else
	{
		tick++; // else, just increment the ticks
	}
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
	{
		return; // return immediately
	}
	if (getWorld()->playerInRadius(this, 4) && !isVisible()) // if not currently visible and tunnelMan is 4 away
	{
		setVisible(true); // make the barrel visible
		return;
	}
	if (getWorld()->playerInRadius(this, 3)) // if barrel is 3 units away from tunnelMan, (and visible)
	{
		isDead(); // the barrel "activates" and the state is now dead
		getWorld()->increaseScore(1000); // increases player's score by 1000
		getWorld()->playSound(SOUND_FOUND_OIL);
		getWorld()->decreaseBarrel(); // barrels left is decreased; once all are picked up, level is complete
		return;
	}
}

// .............................. GOLD CLASS ..............................

Gold::Gold(StudentWorld* w, int xCoords, int yCoords, bool visible, bool dropped) // constructor
	: Goodies(w, TID_GOLD, xCoords, yCoords)
{
	setVisible(visible);
	goldDropped = dropped;
};

void Gold::doSomething()
{
	if (!isAlive()) // if the gold is not currently alive,
	{
		return; // return immediately
	}
	if (!isVisible() && getWorld()->playerInRadius(this, 4)) // if the gold isn't visible and tunnelman is within 4 units
	{
		setVisible(true); // make the gold visible
		return;
	}
	if (!goldDropped && getWorld()->playerInRadius(this, 3)) // if the gold is pickupable by tunnelMan and within 3 units of a tunnelman
	{
		int goodiesID = getID();
		isDead(); // state is set to dead
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->getPlayer()->addGoodies(goodiesID); // increase the gold by 1 and player score by 10
		return;
	}
	if (goldDropped) // if gold is pickupable by protesters and within 3 units from a protester
	{
		Protestor* greedy = (getWorld()->protesterInRadius(this, 3)); // grab a protester within radius of 3
		if (greedy != nullptr) // as long as it is a real protester,
		{
			isDead(); // set the gold to a dead state
			greedy->bribing(); // either stuns or gets a protester to leave
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
		getWorld()->playSound(SOUND_GOT_GOODIE);
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
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->getPlayer()->addGoodies(goodiesID); // increase the water by 5 and player score by 100
		return;
	}
	int lvl = getWorld()->getLevel();
	int T = max(100, 300 - (10 * lvl));
	disappear(T); // water is always in temp state so it will be removed at the end of the tick
}
