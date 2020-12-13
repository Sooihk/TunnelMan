#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"

#include <string>
#include <vector>
#include <algorithm>
#include <queue>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

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
	StudentWorld(std::string assetDir); // constructor
	virtual ~StudentWorld(); // destructor

	virtual int init(); // creates a new oil field and new set of actors
	virtual int move(); // ask all active actors to doSomething(), then removes any actors in the current tick
	virtual void cleanUp(); // destroys level

	void decreaseProtester(); // function which removes a protester who is leaving the field
	void movingtoExitPoint(Protester* pointer); // function which sets protester one square closer to it's exit point
	

	std::vector<Actor*> getActors() // vector container to hold pointers to actors
	{
		return actors;
	}
	
	// check functions
	bool diggingEarth(int col, int row); // delete earth element
	bool checkEarth(int col, int row); // check to see if Earth is in current index
	bool earthAbove(int col, int row); // checks the row above or below the actor
	bool checkBoulder(int col, int row, int radius = 3);
	bool canActorMoveThisDirection(int x, int y, GraphObject::Direction dir); // if actor can move in stated direction
	bool actorsInRadius(int x, int y, int radius); // checks if actors are within the radius
	bool inRadiusAux(int x1, int x2, int y1, int y2, int radius); // check if inside the radius
	bool playerInRadius(Actor* a, int radius);
	bool checkGoodies(int x, int y, int radius); // looks for oil or gold near
	
	Protester* protesterInRadius(Actor* a, int radius);
	TunnelMan* getPlayer();
	void decreaseBarrel() { numOfBarrels--; }
	void addGameItems(int num, char letter);
	void addGoodies();
	void addActor(Actor* actor);

	GraphObject::Direction cellphoneSignalDirection(Protestor* pointer, int M); // function giving direction where TunnelMan is to protestor within 15 moves away
private:
	bool theFirstTick;
	int tickSincePreviousIteration;
	int numberofActiveProtesters; // number of protesters alive in field
	int numOfBarrels; // number of barrels left in field

	std::vector<Actor*> actors; // vector containing pointers to actor classes
	TunnelMan* tunnelPlayer; // pointer to TunnelMan class
	

	int queueMaze[64][64]; // 2d array maze 
	struct queueGrid
	{
		int x; 
		int y;
		queueGrid(int column, int row) : x(column), y(row) {}
	};
	Earth* earthArray[64][64]; // array to hold earth elements
};

#endif // STUDENTWORLD_H_
