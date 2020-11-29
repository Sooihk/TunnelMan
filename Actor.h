#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;
class TunnelMan;

class Actor : public GraphObject
{
private:
	bool aliveStat;  // variable to state if object is alive or dead
	StudentWorld* newWorld; // pointer to StudentWorld class object
public:
	// constructor
	Actor(StudentWorld* w, int imageNum, int xCoords, int yCoords, Direction move, double size, unsigned int depth)
		: GraphObject(imageNum, xCoords, yCoords, move, size, depth)
	{
		setVisible(true);
		aliveStat = true;
		newWorld = w;
	};

	virtual ~Actor() {}; // destructor

	virtual void doSomething() = 0; // doSomething function that gets overridden

	bool actorAlive() const { return aliveStat; }; //returns status of object

	//setter function
	void settingWorld(StudentWorld* gameWorld){ newWorld = gameWorld;};

	//void setAlive(bool a) {	aliveStatus = a; } // sets aliveStatus
	//bool getAlive() { return aliveStatus; } // sets aliveStatus

	//getter function
	StudentWorld* getWorld() const { return newWorld; }; 

};

class Earth : public Actor {
private:
public:
	Earth(StudentWorld* w, int xCoords, int yCoords) : Actor(w, TID_EARTH, xCoords, yCoords, right, 0.25, 3) // constructor
	{
		setVisible(true);
	};
	virtual void doSomething() {}; // necessary to make Earth class not a abstract data class
	virtual ~Earth() {}; // destructor

};

class TunnelMan : public Actor {
private:
	int hitPoints = 10;
	int water = 5;
	int sonarCharge = 1;
	int goldNuggets = 0;

public:
	TunnelMan(StudentWorld* gameWorld); // prototype

	virtual ~TunnelMan() {}; // destructor

	virtual void doSomething();

	// getter functions
	int getHitPoints() { return hitPoints; }
	int getWater() { return water; }
	int getSonarCharge() { return sonarCharge; }
	int getGoldNuggest() { return goldNuggets; }

};

// The functions for number of boulders, gold nuggets, and barrels of oil in each level
// Boulders: int B = min(current_level_number / 2 + 2, 9)
// Gold Nuggets: int G = max(5-current_level_number / 2, 2)
// Barrels of oil: int L = min(2 + current_level_number, 21)


#endif // ACTOR_H_
