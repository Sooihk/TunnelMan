#include "StudentWorld.h"

#include <string>
#include <vector>
#include <algorithm>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// .............................. STUDENTWORLD CLASS ..............................

int StudentWorld::init() // creates oil field and tunnelman
{

	tunnelPlayer = new TunnelMan(this); // create TunnelMan object

	for (int x = 0; x < VIEW_WIDTH; x++) // x is column
	{
		vector<Earth*> e;
		for (int y = 0; y < VIEW_HEIGHT - 4; y++) // y is row
		{
			if (!(y >= tunnel_Ystart && y <= tunnel_Yend
				&& x >= tunnel_Xstart && x <= tunnel_Xend))
				// if x,y is not in spot where the initial tunnel is
			{
				e.push_back(new Earth(this, x, y));
			}
		}
		earth.push_back(e); // push vector of rows into a column
	}

	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() // tells all actors in the current tick to doSomething()
{
	tunnelPlayer->doSomething(); // ask TunnelMan object to do something

	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() // delete the level
{
	delete tunnelPlayer; // delete TunnelMan object

	// delete Earth vector array
	for (auto it = earth.begin(); it != earth.end();)
	{
		for (auto it2 = (*it).begin(); it2 != (*it).end();)
		{
			delete* it2;
			it2 = (*it).erase(it2); // returns next element to iterator
		}
		it = earth.erase(it); // delete element and move on to next one
	}

	for (auto it = actors.begin(); it != actors.end();)
	{
		delete* it; // delete object pointer points to
		it = actors.erase(it); // delete current pointer object and move on to next element
	}

}

bool StudentWorld::checkEarth(int col, int row) const // creating the initial tunnel 
{
	// checking to see if index contains earth
	if ((col >= tunnel_Xstart && col <= tunnel_Xend && row >= tunnel_Ystart && row <= tunnel_Yend)
		|| row < 0 || row > earth_Length)
	{
		return false;
	}
	return true;
}

void StudentWorld::diggingEarth() // digs earth
{
	int row = tunnelPlayer->getY(); //get current row (y-axis) where TunnelMan is
	int column = tunnelPlayer->getX(); //get current column (x-axis) where TunnelMan is

	Actor::Direction tunnelmanDir = tunnelPlayer->getDirection(); // get direction of tunnelman

	switch (tunnelmanDir)
	{
	case Actor::Direction::up:
		for (int i = 0; i < 4; i++) // digs the next four indexes in the current direction
		{
			if (checkEarth(column + i, row + 3)) // checks if we're in the tunnel
			{
				if (earth[column + i][row + 3]) // checks for earth that has been dug already
				{
					delete earth[column + i][row + 3]; // deletes earth 3 indexes up from the original TunnelMan index 4 times
					earth[column + i][row + 3] = nullptr;
					playSound(SOUND_DIG);
				}
			}

		}
	case Actor::Direction::down:
		for (int i = 0; i < 4; i++) // digs the next four indexes in the current direction
		{
			if (checkEarth(column + i, row)) // checks if we're in the tunnel
			{
				if (earth[column + i][row]) // checks for earth that has been dug already
				{
					delete earth[column + i][row]; // deletes earth from the original location down of TunnelMan index 4 times
					earth[column + i][row] = nullptr;
					playSound(SOUND_DIG);
				}
			}
		}
	case Actor::Direction::right:
		for (int i = 0; i < 4; i++) // digs the next four indexes in the current direction
		{
			if (checkEarth(column + 3, row + i)) // checks if we're in the tunnel
			{
				if (earth[column + 3][row + i]) // checks for earth that has been dug already
				{
					delete earth[column + 3][row + i]; // deletes earth 3 indexes right from the original TunnelMan index 4 times
					earth[column + 3][row + i] = nullptr;
					playSound(SOUND_DIG);
				}
			}
		}
	case Actor::Direction::left:
		for (int i = 0; i < 4; i++) // digs the next four indexes in the current direction
		{
			if (checkEarth(column, row + i)) // checks if we're in the tunnel
			{
				if (earth[column][row + i]) // checks for earth that has been dug already
				{
					delete earth[column][row + i]; // deletes earth from the original location left of TunnelMan index 4 times
					earth[column][row + i] = nullptr;
					playSound(SOUND_DIG);
				}
			}
		}
	}
}

bool StudentWorld::aboveOrBelowEarth(int x, int y)
{
	for (int i = x; i < x + 4; i++)
		if (earth[i][y] != nullptr)
			return true;
	return false;
}

bool StudentWorld::checkBoulder(int x, int y, int radius)
{
	for (vector<Actor*>::iterator it = actors.begin(); it != actors.end(); ++it)
		if((*it)->getID() == TID_BOULDER && inRadius(x,y,(*it)->getX(), (*it)->getY(), radius))
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