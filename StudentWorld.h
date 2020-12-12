#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"

#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

const static int earth_Width = 63;
const static int earth_Length = 59;
const static int tunnel_Xstart = 30;
const static int tunnel_Ystart = 4;
const static int tunnel_Xend = 33;
const static int tunnel_Yend = 59;

class Actor;
class TunnelMan;
class Earth;
class Protester;

// .............................. STUDENTWORLD CLASS ..............................

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir) : GameWorld(assetDir)
	{

	}
	virtual ~StudentWorld()
	{
		cleanUp();
	};

	virtual int init(); // creates a new oil field and new set of actors

	virtual int move(); // ask all active actors to doSomething(), then removes any actors in the current tick

	virtual void cleanUp(); // destroys level

	void diggingEarth(); // function where tunnel man digs Earth

	std::vector<Actor*> getActors() // vector container to hold pointers to actors
	{
		return actors;
	}
	std::vector<std::vector<Earth*>> getEarth() // vector container to hold earth objects
	{
		return earth;
	}

	bool checkEarth(int col, int row) const; // check to see if Earth is in current index

	bool aboveOrBelowEarth(int x, int y); // checks the row above or below the actor

	bool checkBoulder(int x, int y, int radius = 3);

	bool inRadius(int x1, int x2, int y1, int y2, int radius); // check if inside the radius

	bool playerInRadius(Actor* a, int radius);

	Protester* protesterInRadius(Actor* a, int radius);

	TunnelMan* getPlayer() { return tunnelPlayer;}

private:
	std::vector<Actor*> actors; // vector containing pointers to actor classes
	TunnelMan* tunnelPlayer = nullptr; // pointer to TunnelMan class
	std::vector <std::vector< Earth*>> earth; // vector container holding all earth objects
};

#endif // STUDENTWORLD_H_
