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


StudentWorld::StudentWorld(string assetDir) :GameWorld(assetDir)
{
	theFirstTick = true;
	tickSincePreviousIteration = 0;
	numberofActiveProtestors = 0;
	tunnelPlayer = nullptr;
	numberofBarrels = 0;
}

StudentWorld::~StudentWorld()
{
	cleanUp();
}

TunnelMan* StudentWorld::getPlayer()
{
	return tunnelPlayer;
}

int StudentWorld::init() // creates oil field and tunnelman
{
	// new field reset all variables
	numberofBarrels = 0;
	numberofActiveProtestors = 0;
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
		if (it->isAlive()) //check to see if actor is alive
		{
			it->doSomething();
		}
		if (!tunnelPlayer->isAlive()) // check if tunnelMan player is alive
		{
			decLives(); // decrease life of player
			return GWSTATUS_PLAYER_DIED;
		}
		if (numberofBarrels == 0) // if player completed level by obtaining all the level's barrels
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

void StudentWorld::decreaseProtestor()
{
	numberofActiveProtestors--;
}

bool StudentWorld::canActorMoveThisDirection(int x, int y, GraphObject::Direction dir)
{
	// switch case returning true or false based upon actor's movement 
	switch (dir)
	{
		// case where actor can move left
	case GraphObject::left:
	{
		return();
	}
	}
}
// queue based maze searching, exploring the oldest x,y location inserted into the queue first
void StudentWorld::movingtoExitPoint(Protestor* pointer)
{
	// populate queue based maze with 0s
	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 64; j++)
		{
			queueMaze[i][j] = 0;
		}
	}

	//get protestor coordinates
	int xcoord = pointer->getX();
	int ycoord = pointer->getY();

	queue<queueGrid> temp;
	//mark exit point in queue array
	temp.push(queueGrid(60, 60));
	queueMaze[60][60] = 1;

	// removing the top point from the queue and looking in 4 directions to seee if there is a non wall space
	while (!temp.empty())
	{
		queueGrid temp2 = temp.front(); // front item of queue
		temp.pop();// remove top point of queue
		int x = temp2.x;
		int y = temp2.y;

		// If slot to the west is open and undiscovered
		if ()
	}

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

Protester* StudentWorld::protesterInRadius(Actor* a, int radius)
{
	for (vector<Actor*>::iterator it = actors.begin(); it != actors.end(); ++it)
		if (((*it)->getID() == TID_PROTESTER || (*it)->getID() == TID_HARD_CORE_PROTESTER)
			&& inRadius(a->getX(), a->getY(), (*it)->getX(), (*it)->getY(), radius))
		{
			return dynamic_cast<Protester*> (*it);
		}
	return nullptr;
}
// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
