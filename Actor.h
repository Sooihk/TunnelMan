#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject {
private:
	bool aliveStatus;
public:
	Actor(int imageNum, int xCoords, int yCoords, Direction move = none) // constructor
		: GraphObject(imageNum, xCoords, yCoords, move)
	{
		setVisible(true);
		aliveStatus = true;
	};

	virtual ~Actor(); // destructor

	virtual void doSomething() = 0; // doSomething function that gets overridden

	void setAlive(bool a) {	aliveStatus = a; } // sets aliveStatus

	bool getAlive() { return aliveStatus; } // sets aliveStatus

};


#endif // ACTOR_H_
