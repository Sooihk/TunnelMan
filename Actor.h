#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"
#include "StudentWorld.h"
#include <string>

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

// .............................. ACTOR CLASS ..............................

class Actor : public GraphObject
{
private:
	bool actorAlive;  // variable to state if object is alive or dead
	StudentWorld* newWorld; // pointer to StudentWorld class object
public:
	// constructor
	Actor(StudentWorld* w, int imageNum, int xCoords, int yCoords, Direction move, double size, unsigned int depth);
	virtual ~Actor(); // destructor
	virtual void doSomething() = 0; // doSomething function that gets overridden
	virtual void actorAnnoyed(int health) {};
	bool isAlive(); // to return whether actor is alive or dead
	void isDead(); // set actor to dead
	void moveTowards(int x, int y); // move actor to desired location
	StudentWorld* getWorld();
};

// .............................. HUMAN CLASS ..............................

class Human : public Actor
{
private:
	int human_HP;
public:
	Human(StudentWorld* w, int imageNum, int xCoords, int yCoords, Direction move, int health);
	int getHealthPoints();
	void decreaseHealthPoints(int damage);
	virtual void moveTowardsDirection(Direction dir) = 0;
	virtual void actorAnnoyed(int health) = 0;

};

// .............................. EARTH CLASS ..............................

class Earth : public Actor {
private:
public:
	Earth(StudentWorld* w, int xCoords, int yCoords); // constructor
	virtual void doSomething(); // necessary to make Earth class not a abstract data class
};

// .............................. TUNNELMAN CLASS ..............................

class TunnelMan : public Human
{
private:
	int water;
	int sonarCharge;
	int goldNuggets;

public:
	TunnelMan(StudentWorld* gameWorld); // prototype
	virtual void moveTowardsDirection(Direction dir); // move TunnelMan towards that direction
	virtual void doSomething();
	virtual void actorAnnoyed(int damage);
	void addGoodies(int goodies); // increase sonar or water or gold count and increase score
	void shootWater(); // shoots water if there isnt earth
	void shootWaterAux(int x, int y); // aux function for squirtWater()

	// getter functions
	int getWater() { return water; }
	int getSonarCharge() { return sonarCharge; }
	int getGoldNugget() { return goldNuggets; }
};

// .............................. PROTESTER CLASS ..............................


class Protestor : public Human
{
public:
	Protestor(StudentWorld* gameWorld, int imageNum, int health); // constructor
	virtual void doSomething();
	virtual void actorAnnoyed(int health);

	void pickDirectionToTurn();
	void randomNumberMoves();
	virtual void moveTowardsDirection(Direction dir);
	void bribing();
	void getStunned();

	bool protestorFacingTunnelMan();
	bool straightTowardsTunnelMan(Direction dir);
	bool protestoratIntersection();

	Direction directionTowardsTunnelMan();
	Direction randomNewDirection();

private:
	bool toLeave;
	int ticksToWaitBetweenMoves;
	int numofSquaresToMoveInCurrentDirection;
	int ticksTillYell;
	int ticksSincePreviousTurn;

};

class Regular_Protestor : public Protestor
{
public:
	Regular_Protestor(StudentWorld* world);
};

class Hardcore_Protestor : public Protestor
{
public:
	Hardcore_Protestor(StudentWorld* world);
};

// .............................. BOULDER CLASS ..............................

class Boulder : public Actor
{
private:
	bool currentlyStable;
	int ticks;
	bool fallingDown;
public:
	Boulder(StudentWorld* w, int xCoords, int yCoords);
	virtual void doSomething();
	void annoyPerson();
};

// .............................. SQUIRT CLASS ..............................

class Squirt : public Actor
{
private:
	int travel;
public:
	Squirt(StudentWorld* w, int xCoords, int yCoords, Direction dir);
	virtual ~Squirt() {};
	virtual void doSomething();
	bool hitProtesters();
	bool checkForObject(int x, int y);
};

// .............................. GOODIES CLASS ..............................

class Goodies : public Actor
{
private:
	int tick;
public:
	Goodies(StudentWorld* w, int imageNum, int xCoords, int yCoords);
	virtual ~Goodies() {};
	virtual void doSomething() = 0;
	void disappear(int t);

};

// .............................. [BARREL] OIL CLASS ..............................

class Oil : public Goodies
{
public:
	Oil(StudentWorld* w, int xCoords, int yCoords);
	virtual ~Oil() {};
	virtual void doSomething();
};

// .............................. GOLD CLASS ..............................

class Gold : public Goodies
{
private:
	bool goldDropped;
public:
	Gold(StudentWorld* w, int xCoords, int yCoords, bool visible, bool dropped);

	virtual ~Gold() {};

	virtual void doSomething();

};

// .............................. SONAR CLASS ..............................
class Sonar : public Goodies
{
public:
	Sonar(StudentWorld* w, int xCoords, int yCoords);

	virtual ~Sonar() {};

	virtual void doSomething();

};

// .............................. WATER CLASS ..............................

class Water : public Goodies {
public:
	Water(StudentWorld* w, int xCoords, int yCoords);
	virtual ~Water() {};
	virtual void doSomething();

};


#endif // ACTOR_H_
