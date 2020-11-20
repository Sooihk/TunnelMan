#include "GameController.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

  // If your program is having trouble finding the Assets directory,
  // replace the string literal with a full path name to the directory,
  // e.g., "Z:/CS32/TunnelMan/Assets" or "/Users/fred/cs32/TunnelMan/Assets"

// Test Comment!
// James comment 
// Sherry's comment
// Sherry's second comment 
// James own comment 
// Sherry's new commment
// testing 1
// testing 2
// testing 3
// testing 4
// testing 5
// testing 6
// testing 7
// testing 8
// testing 9
// testing 10 
// testing 11
// testing 12
// testing 14
// testing 13

const string assetDirectory = "Assets"; 

class GameWorld;

GameWorld* createStudentWorld(string assetDir = "");

int main(int argc, char* argv[])
{
	{
		string path = assetDirectory;
		if (!path.empty())
			path += '/';
		const string someAsset = "dig1.tga";
		ifstream ifs(path + someAsset);
		if (!ifs)
		{
			cout << "Cannot find " << someAsset << " in ";
			cout << (assetDirectory.empty() ? "current directory"
											: assetDirectory) << endl;
			return 1;
		}
	}

	srand(static_cast<unsigned int>(time(nullptr)));

	GameWorld* gw = createStudentWorld(assetDirectory);
	Game().run(argc, argv, gw, "TunnelMan");
}
