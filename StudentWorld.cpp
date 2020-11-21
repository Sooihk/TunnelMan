#include "StudentWorld.h"

#include <string>
#include <vector>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
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
				e.push_back(new Earth(y, x));
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
// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
