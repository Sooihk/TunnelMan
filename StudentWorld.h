#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <algorithm>

// edit new 
// Hi there new edit, updated 11/18
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

//Edit #3

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir)
	{}
	virtual ~StudentWorld() {};


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
	// new changes
private:
	std::vector<Actor*> actors; // vector containing pointers to actor classes
	TunnelMan* tunnelplayer; // pointer to TunnelMan class
};

#endif // STUDENTWORLD_H_
