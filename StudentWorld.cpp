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
	// updatetext 
	setDisplaytext();

	// Give each Actor a chance to do something
	vector<Actor*>::iterator passby;
	for (passby = actors.begin(); passby != actors.end(); passby++)
	{
		if ((*passby)->isAlive()) //check to see if actor is alive
		{
			(*passby)->doSomething();
		}
		if (!tunnelPlayer->isAlive()) // check if tunnelMan player is alive
		{
			decLives(); // decrease life of player
			return GWSTATUS_PLAYER_DIED;
		}

	}
	tunnelPlayer->doSomething(); // ask TunnelMan object to do something
	protestorAdded();

	// clean dead actors
	for (passby = actors.begin(); passby != actors.end();) // for loop until iterator meets end
	{
		if (!(*passby)->isAlive()) // check to see if actor is dead
		{
			delete* passby;
			passby = actors.erase(passby);
		}
		else // else iterate to next actor in vector
		{
			passby++;
		}
	}
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

// function that displays game info: score, level, lives...
void StudentWorld::setDisplaytext()
{
	int level = getLevel();
	int lives = getLives();
	int health = tunnelPlayer->getHealthPoints();
	int score = getScore();
	int gold = tunnelPlayer->getGoldNugget();
	int sonar = tunnelPlayer->getSonarCharge();
	int squirts = tunnelPlayer->getWater();

	stringstream s;

	s.fill('0');
	s << "Scr: ";
	s << setw(6) << score;

	s.fill(' ');
	s << " Lvl: ";
	s << setw(2) << level;

	s << " Lives: ";
	s << setw(1) << lives;

	s << "  Hlth: ";
	s << setw(3) << health * 10;
	s << '%';

	s << "  Wtr: ";
	s << setw(2) << squirts;

	s << "  Gld: ";
	s << setw(2) << gold;

	s << "  Sonar: ";
	s << setw(2) << sonar;

	s << "  Oil Left: ";
	s << setw(2) << numOfBarrels;

	string x = s.str();

	//string s = formatText(score, level, lives, health, squirts, gold, sonar, numOfBarrels);
	setGameStatText(x);

}

//string StudentWorld::formatText(int score, int level, int lives, int health, int squirts, int gold, int sonar, int barrels)
//{
//	stringstream s;
//
//	s.fill('0');
//	s << "Scr: ";
//	s << setw(6) << score;
//
//	s.fill(' ');
//	s << " Lvl: ";
//	s << setw(2) << level;
//
//	s << " Lives: ";
//	s << setw(1) << lives;
//
//	s << "  Hlth: ";
//	s << setw(3) << health * 10;
//	s << '%';
//
//	s << "  Wtr: ";
//	s << setw(2) << squirts;
//
//	s << "  Gld: ";
//	s << setw(2) << gold;
//
//	s << "  Sonar: ";
//	s << setw(2) << sonar;
//
//	s << "  Oil Left: ";
//	s << setw(2) << barrels;
//
//	return s.str();
//}

// checks if actors are within the radius
bool StudentWorld::actorsInRadius(int x, int y, int radius)
{
	vector<Actor*>::iterator it;
	for (it = actors.begin(); it != actors.end(); ++it) // loops through vector containing actors
	{
		if (inRadiusAux(x, y, (*it)->getX(), (*it)->getY(), radius)) // calls inRadiusAux to see if any actors are within radius
		{
			return true;
		}
	}
	return false;
}

// check if inside the radius
bool StudentWorld::inRadiusAux(int x1, int x2, int y1, int y2, int radius)
{
	if (pow((x2 - x1), 2) + pow((y2 - y1), 2) <= radius)
		return true;
	return false;
}

// looks for oil or gold nearby
bool StudentWorld::checkGoodies(int x, int y, int radius)
{
	vector<Actor*>::iterator it;
	for (it = actors.begin(); it != actors.end(); ++it) // loops through vector containing actors
	{
		if ((*it)->getID() == TID_GOLD || (*it)->getID() == TID_BARREL) // checks nearby radius for any gold or barrels of oil
		{
			if (inRadiusAux(x, (*it)->getX(), y, (*it)->getY(), radius))
				(*it)->setVisible(true);
		}
	}
	return false;
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

// function adding protestors on field
void StudentWorld::protestorAdded()
{
	// #1 a new protestor may be added to field after at least T ticks have passed since the last protestor was added
	int T = max(25, 200 - (int)getLevel());
	// #2 maximum number protestors is P 
	int P = fmin(15, 2 + (int)getLevel() * 1.5); // needed fmin instead of min since min would not take (int, double) as parameter
	int probabilityOfHardcore = min(90, (int)getLevel() * 10 + 30);

	// #3,4 first protestor added during very first tick of each level
	// if appropiate # of ticks passed since last protestor added and # of protestors is less than P 
	if (theFirstTick || (tickSincePreviousIteration > T && numberofActiveProtestors < P))
	{
		int willbenotHardCore = rand() % 100 + 1; // number range from 0-100
		if (willbenotHardCore < probabilityOfHardcore) // if less than probabilityOfHardcore, hardcore protestor is added
		{
			actorAdded(new Hardcore_Protestor(this));
		}
		else // else a regular protestor was added
		{
			actorAdded(new Regular_Protestor(this));
		}
		numberofActiveProtestors++;
		theFirstTick = false;
		tickSincePreviousIteration = 0;
	}
	tickSincePreviousIteration++;
}

// function which decreases number of protestors on field
void StudentWorld::decreaseProtestor()
{
	numberofActiveProtestors--;
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
		break;
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
		break;
	}
	// case whether the actor can move left
	case GraphObject::left:
	{
		// if statement checking if above the actor there isn't an earth and boulder object and current x
		// isnt the exit point
		if (!checkBoulder(x - 1, y) && !checkEarth(x - 1, y) && x != 60)
		{
			return true;
		}
		break;
	}
	// case whether the actor can move right
	case GraphObject::right:
	{
		// if statement checking if above the actor there isn't an earth and boulder object and current x
		// isnt the exit point
		if (!checkBoulder(x + 1, y) && !checkEarth(x + 1, y) && x != 60)
		{
			return true;
		}
		break;
	}
	case GraphObject::none:
	{
		return false;
		break;
	}
	}
	return false;
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
	//mark starting point in queue array as the protestor exit point
	temp.push(queueGrid(60, 60));
	queueMaze[60][60] = 1; // finish point

	// removing the top point from the queue and looking in 4 directions to seee if there is a non wall space
	// starting point value is 1, keep adding +1 to value on each index discovered (1,2,3...)
	while (!temp.empty()) // loop until queue is empty
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
bool StudentWorld::checkEarth(int col, int row) // check to see if Earth is in current index
{
	for (int i = col; i < col + 4; i++) {

		for (int j = row; j < row + 4; j++) {

			if (earthArray[i][j] != nullptr)
			{
				return true;
			}
		}
	}
	return false;
}

// function to check if actor is above earth
bool StudentWorld::earthAbove(int x, int y)
{
	for (int i = x; i < x + 4; i++)
		if (earthArray[i][y] != nullptr)
		{
			return true;
		}
	return false;
}

bool StudentWorld::checkBoulder(int x, int y, int radius)
{
	vector<Actor*>::iterator it;
	for (it = actors.begin(); it != actors.end(); ++it)
		if ((*it)->getID() == TID_BOULDER && inRadius(x, y, (*it)->getX(), (*it)->getY(), radius))
		{
			return true;
		}
	return false;
}

// function telling which direction protestor to move if tunnelman cellphone signal within 15 moves away
GraphObject::Direction StudentWorld::cellphoneSignalDirection(Protestor* pointer, int M)
{
	// use queue-based maze searching algorithm 
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
	//mark starting point in queue array as TunnelMan's current location 
	int playerX = getPlayer()->getX();
	int playerY = getPlayer()->getY();
	temp.push(queueGrid(playerX, playerY)); // initialize queue with start index
	queueMaze[playerX][playerY] = 1; // mark start point

	// removing the top point from the queue and looking in 4 directions to seee if there is a non wall space
	// starting point value is 1, keep adding +1 to value on each index discovered (1,2,3...)
	while (!temp.empty()) // loop until queue is empty
	{
		queueGrid temp2 = temp.front(); // front item of queue
		temp.pop();// remove top point of queue
		int x = temp2.x;
		int y = temp2.y;

		// If slot to the EAST is open and undiscovered
		if (queueMaze[x + 1][y] == 0 && canActorMoveThisDirection(x, y, GraphObject::right))
		{
			queueMaze[x + 1][y] = 1 + queueMaze[x][y];//mark as discovered
			temp.push(queueGrid(x + 1, y)); // insert right location on queue
		}
		// If slot to the WEST is open and undiscovered
		if (queueMaze[x - 1][y] == 0 && canActorMoveThisDirection(x, y, GraphObject::left))
		{
			queueMaze[x - 1][y] = 1 + queueMaze[x][y];//mark as discovered
			temp.push(queueGrid(x - 1, y)); // insert left location on queue
		}
		// If slot to the SOUTH is open and undiscovered
		if (queueMaze[x][y - 1] == 0 && canActorMoveThisDirection(x, y, GraphObject::down))
		{
			queueMaze[x][y - 1] = 1 + queueMaze[x][y];//mark as discovered
			temp.push(queueGrid(x, y - 1)); // insert down location on queue
		}
		// If slot to the NORTH is open and undiscovered
		if (queueMaze[x][y + 1] == 0 && canActorMoveThisDirection(x, y, GraphObject::up))
		{
			queueMaze[x][y + 1] = 1 + queueMaze[x][y];//mark as discovered
			temp.push(queueGrid(x, y + 1)); // insert up location on queue
		}
	}

	// if statements to retarce back to point index value 1 (where TunnelMan is)
	// if there is no earth or boulders and queueArray value to the right is less than current
	if (M + 1 >= queueMaze[xcoord][ycoord]) // using the queueMaze numbering, if the value where TunnelMan is within <= 15+1, then can detect cellphone signal
	{
		if (queueMaze[xcoord + 1][ycoord] < queueMaze[xcoord][ycoord] && canActorMoveThisDirection(xcoord, ycoord, GraphObject::right))
		{
			return GraphObject::right; // tell actor to move towards right direction
		}
		// if there is no earth or boulders and queueArray value to the up is less than current
		if (queueMaze[xcoord][ycoord + 1] < queueMaze[xcoord][ycoord] && canActorMoveThisDirection(xcoord, ycoord, GraphObject::up))
		{
			return GraphObject::up; // tell actor to move towards up direction
		}
		// if there is no earth or boulders and queueArray value to the left is less than current
		if (queueMaze[xcoord - 1][ycoord] < queueMaze[xcoord][ycoord] && canActorMoveThisDirection(xcoord, ycoord, GraphObject::left))
		{
			return GraphObject::left; // tell actor to move towards left direction
		}
		// if there is no earth or boulders and queueArray value to the down is less than current
		if (queueMaze[xcoord][ycoord - 1] < queueMaze[xcoord][ycoord] && canActorMoveThisDirection(xcoord, ycoord, GraphObject::down))
		{
			return GraphObject::down; // tell actor to move towards down direction
		}
	}
	//  else
	return GraphObject::none;
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
	vector<Actor*>::iterator it;
	for (it = actors.begin(); it != actors.end(); ++it)
		if (((*it)->getID() == TID_PROTESTER || (*it)->getID() == TID_HARD_CORE_PROTESTER)
			&& inRadius(a->getX(), a->getY(), (*it)->getX(), (*it)->getY(), radius))
		{
			return dynamic_cast<Protestor*> (*it);
		}
	return nullptr;
}

void StudentWorld::actorAdded(Actor* actor)
{
	actors.push_back(actor); // adding new actor to vector
}

void StudentWorld::addGoodies()
{
	int col, row;
	int G = (getLevel() * 25) + 300; // given in PDF
	if (int(rand() % G) + 1 == 1) // 1 in G chance of spawning a sonar or water kit
	{
		if (int(rand() % 5) + 1 == 1) // 1 in 5 chance for a Sonar kit
			actorAdded(new Sonar(this, 0, 60));
		else // 4 in 5 chance for a Water 
		{
			do {
				col = rand() & 60 + 1; // grab a random col
				row = rand() & 60 + 1; // grab a random row
			} while (checkEarth(col, row)); // while there is Earth in the current index
			actorAdded(new Water(this, col, row));
		}
	}
}

void StudentWorld::addGameItems(int num, char letter) // addBoulderorGoldorBarrel();
{
	int col = 0;
	int row = 0;
	for (int i = 0; i < num; i++)
	{
		do {
			col = rand() % 60 + 1; // grab a random x value range[0-60]
			if (letter == 'B')
				// Boulders must be distributed between x=0,y=20 and x=60,y=56, inclusive
				// rand() format rand()%((userEnd - userBeg) + 1) + userBeg; 
				row = rand() % (56 - 20 + 1) + 20;
			else
				// gold and oil must be distributed between x = 0, y = 0 and x = 60, y = 56 inclusive
				row = rand() % (56 - 20 + 1) + 20; // rand() format rand()%((userEnd - userBeg) + 1) + userBeg; 
		} while ((col > 26 && col < 34 && row > 0) || actorsInRadius(col, row, 6)); // checking for within range
		switch (letter)
		{
		case 'B':
		{
			actorAdded(new Boulder(this, row, col));
			break;
		}
		case 'G':
		{
			actorAdded(new Gold(this, row, col, false, false));
			break;
		}
		case 'O':
		{
			actorAdded(new Oil(this, row, col));
			break;
		}
		}
	}
}
