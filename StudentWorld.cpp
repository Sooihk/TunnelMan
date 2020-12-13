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
	int B = min(getLevel() / 2 + 2, 9); // boulders
	int G = max(getLevel() / 2, 2); // gold
	int L = min(2 + getLevel(), 21); // oil
	addGameItems(B, 'B');
	addGameItems(G, 'G');
	addGameItems(L, 'L');

	return GWSTATUS_CONTINUE_GAME;
}


//UNFINISHED
void StudentWorld::addGameItems(int num, char letter) // addBoulderorGoldorBarrel();
{
	int col, row;
	for (int i = 0; i < num; i++)
	{
		do {
			col = rand() % 60 + 1;

		} while (actorsInRadius(col, row, 6) || (x > 26 && x < 34 && y > 0));
	}
}

bool StudentWorld::actorsInRadius(int x, int y, int radius)
{
	for (vector<Actor*>::iterator it = actors.begin(); it != actors.end(); ++it)
	{
		if (inRadius(x, y, (*it)->getX(), (*it)->getY(), radius))
			return true;
	}
	return false;
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
	//mark exit point in queue array
	temp.push(queueGrid(60, 60));
	queueMaze[60][60] = 1; // finish point

	// removing the top point from the queue and looking in 4 directions to seee if there is a non wall space
	while (!temp.empty())
	{
		queueGrid temp2 = temp.front(); // front item of queue
		temp.pop();// remove top point of queue
		int x = temp2.x;
		int y = temp2.y;

		// If slot to the east is open and undiscovered
		if (earthArray[x + 1][y] == 0 && canActorMoveThisDirection(x, y, GraphObject::right))
		{
			earthArray[x + 1][y] = 1 + earthArray[x][y];//mark as discovered
			temp.push(queueGrid(x + 1, y)); // insert right location on queue
		}
		// If slot to the west is open and undiscovered
		if (earthArray[x - 1][y] == 0 && canActorMoveThisDirection(x, y, GraphObject::left))
		{
			earthArray[x - 1][y] = 1 + earthArray[x][y];//mark as discovered
			temp.push(queueGrid(x - 1, y)); // insert left location on queue
		}
		// If slot to the south is open and undiscovered
		if (earthArray[x][y - 1] == 0 && canActorMoveThisDirection(x, y, GraphObject::down))
		{
			earthArray[x][y - 1] = 1 + earthArray[x][y];//mark as discovered
			temp.push(queueGrid(x, y - 1)); // insert down location on queue
		}
		// If slot to the north is open and undiscovered
		if (earthArray[x][y + 1] == 0 && canActorMoveThisDirection(x, y, GraphObject::up))
		{
			earthArray[x][y + 1] = 1 + earthArray[x][y];//mark as discovered
			temp.push(queueGrid(x, y + 1)); // insert up location on queue
		}
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

bool StudentWorld::checkGoodies(int x, int y, int radius)
{
	for (vector<Actor*>::iterator it = actors.begin(); it != actors.end(); ++it)
	{
		if ((*it)->getID() == TID_BARREL || (*it)->getID() == TID_GOLD)
		{
			if (inRadius(x, (*it)->getX(), y, (*it)->getY(), radius))
				(*it)->setVisible(true);
		}
	}
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

void StudentWorld::addGoodies()
{
	int col, row;
	int G = (getLevel() * 25) + 300; // given in PDF
	if (int(rand() % G) + 1 == 1) // 1 in G chance of spawning a sonar or water kit
	{
		if (int(rand() % 5) + 1 == 1) // 1 in 5 chance for a Sonar kit
			addActor(new Sonar(this, 0, 60));
		else // 4 in 5 chance for a Water 
		{
			do {
				col = rand() & 60 + 1; // grab a random col
				row = rand() & 60 + 1; // grab a random row
			} while (checkEarth(col, row)); // while there is Earth in the current index
			addActor(new Water(this, col, row));
		}
	}
}

void StudentWorld::addActor(Actor* actor)
{
	actors.push_back(actor); // adding to the vector
}