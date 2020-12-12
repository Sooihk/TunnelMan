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
	virtual ~Actor() {}; // destructor
	virtual void doSomething() = 0; // doSomething function that gets overridden
	virtual void actorAnnoyed(int health) {};
	bool isAlive(); 
	void isDead(); 
	void moveTowards(int x, int y);
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
	virtual void doSomething() {}; // necessary to make Earth class not a abstract data class
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

	virtual ~TunnelMan() {}; // destructor
	virtual void moveTowardsDirection(Direction dir);
	virtual void doSomething();
	virtual void actorAnnoyed(int damage);

	// getter functions
	int getWater() { return water; }
	int getSonarCharge() { return sonarCharge; }
	int getGoldNuggest() { return goldNuggets; }

};

// .............................. PROTESTER CLASS ..............................


class Protestor : public Human
{
public:
	Protestor(StudentWorld* gameWorld, int imageNum, int health); // constructor
	virtual void doSomething();
	virtual void actorAnnoyed(int health);
	virtual void takeBribe(); 

	void stunLocked();
	void pickDirectionToTurn();
	void randomNumberMoves();
	virtual void moveTowardsDirection(Direction dir);
	
	bool facingPlayer();
	bool straightTowardsPlayer(Direction dir);
	bool atIntersection();
	
	Direction directionsTowardsTunnelMan();
	Direction randomSetDirection();

private:
	bool toLeave;
	int ticksToWaitBetweenMoves;
	int numofSquaresMove;
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

class Boulder : public Actor {
private:
	bool stable;
	int ticks;
	bool falling;
public:
	Boulder(StudentWorld* w, int xCoords, int yCoords);
	virtual void doSomething();
	void annoyPerson();
};

// .............................. GOODIES CLASS ..............................

class Goodies : public Actor {
private:
	bool pickupAble; // true for tunnelman, false for protesters, or other way around
	bool permanent; // true for permanent, false for temporary
public:
	Goodies(StudentWorld* w, int imageNum, int xCoords, int yCoords);
	virtual ~Goodies() {};
	virtual void doSomething() = 0;

};

// .............................. OIL CLASS ..............................

class Oil : public Goodies {
public:
	Oil(StudentWorld* w, int xCoords, int yCoords);

	virtual ~Oil() {};

	virtual void doSomething();
};

// .............................. GOLD CLASS ..............................

class Gold : public Goodies {
private:
public:
	Gold(StudentWorld* w, int xCoords, int yCoords);

	virtual ~Gold() {};

	virtual void doSomething();

};

// .............................. SONAR CLASS ..............................

class Sonar : public Goodies {
private:
	bool pickupAble;
	bool permanent;
public:
	Sonar(StudentWorld* w, int xCoords, int yCoords);

	virtual ~Sonar() {};

	virtual void doSomething();

};

// .............................. WATER CLASS ..............................

class Water : public Goodies {
private:
	bool pickupAble;
	bool permanent;
public:
	Water(StudentWorld* w, int xCoords, int yCoords);
	virtual void doSomething();

};

// The functions for number of boulders, gold nuggets, and barrels of oil in each level
// Boulders: int B = min(current_level_number / 2 + 2, 9)
// Gold Nuggets: int G = max(5-current_level_number / 2, 2)
// Barrels of oil: int L = min(2 + current_level_number, 21)



#endif // ACTOR_H_
