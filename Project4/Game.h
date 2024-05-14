#pragma once

#include <string>
#include <fstream>
#include <queue> 
#include <set> 
#include <map> 
#include <utility> 
#include "Initialize.h"
#include "Object.h"



enum Direction
{
	INVALID_DIR = -1,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class Game
{
public:
	Game();
	~Game() = default;

	void run();

	//Cập nhật màn chơi
	void resetLvlData();
	bool loadCurLvl(const std::string level);
	void loadYard(const std::string line, const unsigned int r);
	void update(bool& gameLoop);
	bool loadNextLvl();

	//Xử lý di chuyển
	void moveDog(const Direction direction);
	bool moveManage(const unsigned int r, const unsigned int c, const Direction direction, const States st);
	bool moveObject(const unsigned int r, const unsigned int c, int dX, int dY, const States st);

	//Xử lý events input
	void handleUserInput(bool& gameLoop, bool& reverseDir);
	
	bool checkLvl() const;

private:

	//Xử lý interface
	std::vector<std::vector<Object>> yard;
	SDL_Rect dogPos;

	unsigned int objectHoriSize; 
	unsigned int objectVertiSize; 
	int objectWidth; 
	int objectHeight; 
	Initialize initialize; 

	unsigned int dogCnt; 
	unsigned int sheepCnt;
	unsigned int inBarn;
	unsigned int barnCnt;
	unsigned int lvlTotal;
	unsigned int curLvl;
	
};
