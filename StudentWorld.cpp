#include "StudentWorld.h"
#include "Actor.h"
#include "GraphObject.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <algorithm>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// .............................. STUDENTWORLD CLASS ..............................


StudentWorld::StudentWorld(string assetDir):GameWorld(assetDir)
{
	theFirstTick = true;
	tickSincePreviousIteration = 0;
	numberofActiveProtesters = 0;
	tunnelPlayer = nullptr;
	numOfBarrels = 0;
}

StudentWorld::~StudentWorld()
{
	cleanUp();
}

TunnelMan * StudentWorld::getPlayer()
{
	return tunnelPlayer;
}

int StudentWorld::init() // creates oil field and tunnelman
{
	// new field reset all variables
	numOfBarrels = 0;
	numberofActiveProtesters = 0;
	theFirstTick = true;
	tickSincePreviousIteration = 0;

	// create new oil field
	for (int x = 0; x < VIEW_WIDTH; x++) // x is column
	{
		for (int y = 0; y < VIEW_HEIGHT - 4; y++) // y is row
		{
			// if x,y is not in spot where the initial tunnel is
			if (y<4 || x>tunnel_Xend || x < tunnel_Xstart)
			{
				earthArray[x][y] = new Earth(this, x, y);
			}
		}
	}
	tunnelPlayer = new TunnelMan(this); // create tunnelman player

	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() // tells all actors in the current tick to doSomething()
{
	// updatetext do later

	// Give each Actor a chance to do something
	for (auto it : actors)
	{
		if(it->isAlive()) //check to see if actor is alive
		{
			it->doSomething();
		}
		if (!tunnelPlayer->isAlive()) // check if tunnelMan player is alive
		{
			decLives(); // decrease life of player
			return GWSTATUS_PLAYER_DIED;
		}
		if (numOfBarrels == 0) // if player completed level by obtaining all the level's barrels
		{
			return GWSTATUS_FINISHED_LEVEL;
		}

	}
	tunnelPlayer->doSomething(); // ask TunnelMan object to do something

	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() // delete the level
{
	delete tunnelPlayer; // delete TunnelMan object

	// delete Earth 2d array
	for (int x = 0; x < 64; x++)
	{
		for (int y = 0; y < 60; y++)
		{
			delete earthArray[x][y];
		}
	}
	// delete actors in vector container
	for (auto it = actors.begin(); it != actors.end();)
	{
		delete* it; // delete object pointer points to
		it = actors.erase(it); // delete current pointer object and move on to next element
	}

}

// function which removes earth object 4by4 area occupied by tunnelman, returns true if yes
bool StudentWorld::diggingEarth(int col, int row)
{
	bool destroyedEarth = false;
	// remove earth objects from the 4by4 area occupied by the tunnelman
	for (int i = col; i < col + 4; i++) // use nested for loop to delete 2d earthArray index
	{
		for (int j = row; j < row + 3; j++)
		{
			if (earthArray[i][j] != nullptr) // check if earth was already dugged
			{
				destroyedEarth = true;
				delete earthArray[i][j];
				earthArray[i][j] = nullptr;
			}
		}
	}
	return destroyedEarth;
}

// function which decreases number of protesters on field
void StudentWorld::decreaseProtester()
{
	numberofActiveProtesters--;
}

// function which returns true or false wherein can the actor move the desired direction without hitting a earth or boulder object
bool StudentWorld::canActorMoveThisDirection(int x, int y, GraphObject::Direction dir)
{
	// switch case returning true or false based upon actor's movement 
	switch (dir)
	{
	// case whether the actor can move up
	case GraphObject::up: 
	{
		// if statement checking if above the actor there isn't an earth and boulder object and current x
		// isnt the exit point
		if (!checkBoulder(x, y + 1) && !checkEarth(x, y + 1) && y != 60)
		{
			return true;
		}
	}
	// case whether the actor can move down
	case GraphObject::down:
	{
		// if statement checking if above the actor there isn't an earth and boulder object and current x
		// isnt the exit point
		if (!checkBoulder(x, y - 1) && !checkEarth(x, y - 1) && y != 60)
		{
			return true;
		}
	}
	// case whether the actor can move left
	case GraphObject::left :
	{
		// if statement checking if above the actor there isn't an earth and boulder object and current x
		// isnt the exit point
		if (!checkBoulder(x-1, y) && !checkEarth(x-1, y) && x != 60)
		{
			return true;
		}
	}
	// case whether the actor can move right
	case GraphObject::right:
	{
		// if statement checking if above the actor there isn't an earth and boulder object and current x
		// isnt the exit point
		if (!checkBoulder(x+1, y) && !checkEarth(x+1, y) && x != 60)
		{
			return true;
		}
	}
	case GraphObject::none:
	{
		return false;
	}
	}
	return false;
}

// queue based maze searching, exploring the oldest x,y location inserted into the queue first
void StudentWorld::movingtoExitPoint(Protester* pointer)
{
	// populate queue based maze with 0s
	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 64; j++)
		{
			queueMaze[i][j] = 0;
		}
	}

	//get protester coordinates
	int xcoord = pointer->getX();
	int ycoord = pointer->getY();

	queue<queueGrid> temp; 
	//mark starting point in queue array as the protestor exit point
	temp.push(queueGrid(60, 60));
	queueMaze[60][60] = 1; // finish point

	// removing the top point from the queue and looking in 4 directions to seee if there is a non wall space
	// starting point value is 1, keep adding +1 to value on each index discovered (1,2,3...)
	while (!temp.empty())
	{
		queueGrid temp2 = temp.front(); // front item of queue
		temp.pop();// remove top point of queue
		int x = temp2.x;
		int y = temp2.y;

		// If slot to the east is open and undiscovered
		if (queueMaze[x + 1][y] == 0 && canActorMoveThisDirection(x, y, GraphObject::right))
		{
			queueMaze[x + 1][y] = 1 + queueMaze[x][y];//mark as discovered
			temp.push(queueGrid(x + 1, y)); // insert right location on queue
		}
		// If slot to the west is open and undiscovered
		if (queueMaze[x - 1][y] == 0 && canActorMoveThisDirection(x, y, GraphObject::left))
		{
			queueMaze[x - 1][y] = 1 + queueMaze[x][y];//mark as discovered
			temp.push(queueGrid(x - 1, y)); // insert left location on queue
		}
		// If slot to the south is open and undiscovered
		if (queueMaze[x][y - 1] == 0 && canActorMoveThisDirection(x, y, GraphObject::down))
		{
			queueMaze[x][y - 1] = 1 + queueMaze[x][y];//mark as discovered
			temp.push(queueGrid(x, y - 1)); // insert down location on queue
		}
		// If slot to the north is open and undiscovered
		if (queueMaze[x][y + 1] == 0 && canActorMoveThisDirection(x, y, GraphObject::up))
		{
			queueMaze[x][y + 1] = 1 + queueMaze[x][y];//mark as discovered
			temp.push(queueGrid(x, y + 1)); // insert up location on queue
		}
	}
	// if statements to retarce back to point index value 1 (exit point)
	 // if there is no earth or boulders and queueArray value to the right is less than current
	if (canActorMoveThisDirection(xcoord, ycoord, GraphObject::right) && queueMaze[xcoord + 1][ycoord] < queueMaze[xcoord][ycoord])
	{
		pointer->moveTowardsDirection(GraphObject::right); // tell actor to move towards right direction
	}
	// if there is no earth or boulders and queueArray value to the up is less than current
	if (canActorMoveThisDirection(xcoord, ycoord, GraphObject::up) && queueMaze[xcoord][ycoord + 1] < queueMaze[xcoord][ycoord])
	{
		pointer->moveTowardsDirection(GraphObject::up); // tell actor to move towards up direction
	}
	// if there is no earth or boulders and queueArray value to the left is less than current
	if (canActorMoveThisDirection(xcoord, ycoord, GraphObject::left) && queueMaze[xcoord - 1][ycoord] < queueMaze[xcoord][ycoord])
	{
		pointer->moveTowardsDirection(GraphObject::left); // tell actor to move towards left direction
	}
	// if there is no earth or boulders and queueArray value to the down is less than current
	if (canActorMoveThisDirection(xcoord, ycoord, GraphObject::down) && queueMaze[xcoord][ycoord - 1] < queueMaze[xcoord][ycoord])
	{
		pointer->moveTowardsDirection(GraphObject::down); // tell actor to move towards down direction
	}
	return;

}
bool StudentWorld::checkEarth(int col, int row) // creating the initial tunnel 
{
	for (int i = col; i < col + 4; i++) {

		for (int j = row; j < row + 4; j++) {

			if (earthArray[i][j] != nullptr)
				return true;
		}
	}
	return false;
}

bool StudentWorld::earthAbove(int x, int y)
{
	for (int i = x; i < x + 4; i++)
		if (earthArray[i][y] != nullptr)
			return true;
	return false;
}

bool StudentWorld::checkBoulder(int x, int y, int radius)
{
	for (vector<Actor*>::iterator it = actors.begin(); it != actors.end(); ++it)
		if ((*it)->getID() == TID_BOULDER && inRadius(x, y, (*it)->getX(), (*it)->getY(), radius))
			return true;
	return false;
}

bool StudentWorld::inRadius(int x1, int x2, int y1, int y2, int radius)
{
	if (pow((x2 - x1), 2) + pow((y2 - y1), 2) <= radius)
		return true;
	return false;
}

bool StudentWorld::playerInRadius(Actor* a, int radius)
{
	return inRadius(a->getX(), a->getY(), tunnelPlayer->getX(), tunnelPlayer->getY(), radius);
}

Protestor* StudentWorld::protesterInRadius(Actor* a, int radius)
{
	for (vector<Actor*>::iterator it = actors.begin(); it != actors.end(); ++it)
		if (((*it)->getID() == TID_PROTESTER || (*it)->getID() == TID_HARD_CORE_PROTESTER)
			&& inRadius(a->getX(), a->getY(), (*it)->getX(), (*it)->getY(), radius))
		{
			return dynamic_cast<Protestor*> (*it);
		}
	return nullptr;
}
// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
