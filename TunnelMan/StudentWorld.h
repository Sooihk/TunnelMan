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
class Protestor;
class GraphObject;

// .............................. STUDENTWORLD CLASS ..............................

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir); // constructor
	virtual ~StudentWorld(); // destructor

	virtual int init(); // creates a new oil field and new set of actors
	virtual int move(); // ask all active actors to doSomething(), then removes any actors in the current tick
	virtual void cleanUp(); // destroys level

	void addGoodies(); // adds sonar or water on the field
	void addGameItems(int num, char letter);
	void actorAdded(Actor* actor);
	void protestorAdded(); // adding a protestor to the field 
	void decreaseBarrel() { numOfBarrels--; } // decrease barrel count on field
	void decreaseProtestor(); // function which removes a protestor who is leaving the field
	void movingtoExitPoint(Protestor* pointer); // function which sets protestor one square closer to it's exit point
	void setDisplaytext(); // displays text of game scores and info
	//void formatText(int score, int level, int lives, int health, int squirts, int gold, int sonar, int barrels); // formats game info into a string

	// check functions
	bool diggingEarth(int col, int row); // delete earth element
	bool checkEarth(int col, int row); // check to see if Earth is in current index
	bool earthAbove(int col, int row); // function to check if actor is above earth
	bool checkBoulder(int col, int row, int radius = 3);
	bool canActorMoveThisDirection(int x, int y, GraphObject::Direction dir); // if actor can move in stated direction
	bool actorsInRadius(int x, int y, int radius); // checks if actors are within the radius
	bool inRadiusAux(int x1, int x2, int y1, int y2, int radius); // check if inside the radius 
	bool inRadius(int x1, int x2, int y1, int y2, int radius); // check if inside the radius
	bool playerInRadius(Actor* a, int radius);
	bool checkGoodies(int x, int y, int radius); // looks for oil or gold nearby

	Protestor* protesterInRadius(Actor* a, int radius);
	TunnelMan* getPlayer();
	GraphObject::Direction cellphoneSignalDirection(Protestor* pointer, int M); // function giving direction where TunnelMan is to protestor within 15 moves away
private:
	bool theFirstTick;
	int numOfBarrels; // number of barrels left in field
	int tickSincePreviousIteration;
	int numberofActiveProtestors; // number of protestors alive in field


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
