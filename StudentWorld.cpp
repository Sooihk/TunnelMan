#include "StudentWorld.h"

#include <string>
#include <vector>
#include <algorithm>
using namespace std;

GameWorld* createStudentWorld(string assetDir, TunnelMan *tp)
{
	return new StudentWorld(assetDir, tp);
}

int StudentWorld::init()
{
	
	tunnelPlayer = new TunnelMan(this); // create TunnelMan object

	for (int x = 0; x < VIEW_WIDTH; x++) // x is column
	{
		vector<Earth*> e; 
		for (int y = 0; y < VIEW_HEIGHT-4; y++) // y is row
		{
			if (!(y >= tunnel_Xstart && y <= tunnel_Xend
				&& x >= tunnel_Ystart && x <= tunnel_Yend))
			// if x,y is not in spot where the initial tunnel is
			{
				e.push_back(new Earth(this, y, x));
			}
		}
		earth.push_back(e); // push vector of rows into a column
	}

	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	tunnelPlayer->doSomething(); // ask TunnelMan object to do something

	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
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

bool StudentWorld::checkEarth(int col, int row) const
{
	// checking to see if index contains earth
	if ((col >= tunnel_Xstart && col <= tunnel_Xend && row >= tunnel_Ystart && row <= tunnel_Yend)
		|| row < 0 || row > earth_Length)
	{
		return false;
	}
	return true;
}

void StudentWorld::diggingEarth()
{
	int row = tunnelPlayer->getY(); //get current row (y-axis) where TunnelMan is
	int column = tunnelPlayer->getX(); //get current column (x-axis) where TunnelMan is

	Actor::Direction tunnelmanDir = tunnelPlayer->getDirection(); // get direction of tunnelman

	switch (tunnelmanDir)
	{
		case Actor::Direction::up:
			for (int i = 0; i < 4; i++)
			{
				if (checkEarth(column + i, row + 3))
				{
					if (earth[column + i][row + 3])
					{
						delete earth[column + i][row + 3];
						earth[column + i][row + 3] = nullptr;
						playSound(SOUND_DIG);
					}
				}
			}
		case Actor::Direction::down:
			for (int i = 0; i < 4; i++)
			{
				if (checkEarth(column + i, row))
				{
					if (earth[column + i][row])
					{
						delete earth[column + i][row];
						earth[column + i][row] = nullptr;
						playSound(SOUND_DIG);
					}
				}
			}
		case Actor::Direction::right:
			for (int i = 0; i < 4; i++)
			{
				if (checkEarth(column + 3, row+i))
				{
					if (earth[column + 3][row+i])
					{
						delete earth[column + 3][row+i];
						earth[column + 3][row+i] = nullptr;
						playSound(SOUND_DIG);
					}
				}
			}
		case Actor::Direction::left:
			for (int i = 0; i < 4; i++)
			{
				if (checkEarth(column, row+i))
				{
					if (earth[column][row+i])
					{
						delete earth[column][row+i];
						earth[column][row+i] = nullptr;
						playSound(SOUND_DIG);
					}
				}
			}
	}
}
// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
