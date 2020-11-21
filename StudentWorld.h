#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"

#include <string>
#include <vector>
#include <algorithm>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

const static int earth_Width = 63;
const static int earth_Height = 59;
const static int tunnel_Xstart = 30;
const static int tunnel_Ystart = 4;
const static int tunnel_Xend = 33;
const static int tunnel_Yend = 59;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir)
	{}
	virtual ~StudentWorld() 
	{
		cleanUp();
	};


	virtual int init()
	{
		return GWSTATUS_CONTINUE_GAME;
	}

	virtual int move()
	{
		// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
		// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}

	virtual void cleanUp()
	{
	}
	std::vector<Actor*> getActors()
	{
		return actors;
	}
	std::vector<std::vector<Earth*>> getEarth()
	{
		return earth;
	}
	
private:
	std::vector<Actor*> actors; // vector containing pointers to actor classes
	TunnelMan* tunnelPlayer; // pointer to TunnelMan class
	std::vector <std::vector< Earth*>> earth; 
};

#endif // STUDENTWORLD_H_
