#include "Game.h"

Game::Game()
{
	objectWidth = width;
	objectHeight = height;
	barnCnt = 0;
	sheepCnt = 0;
	inBarn = 0;
	dogCnt = 0;
	curLvl = 0;
	lvlTotal = 3;
	if (loadNextLvl())
		initialize.Render(yard, true, false);
}

void Game::run()
{
	bool gameLoop = true;
	bool isFirst = true;
	bool reverseDir = false;

	//Vòng lặp game
	while (gameLoop)
	{
		handleUserInput(gameLoop, reverseDir);
		initialize.Render(yard, isFirst, reverseDir);
		isFirst = false;
		//Cài đặt FPS
		SDL_Delay(30); 
		update(gameLoop);
	}
}


//Đọc file .txt level và vẽ yard
bool Game::loadCurLvl(const std::string level)
{
	std::ifstream file(level);
	if (!file.is_open())
	{
		return false;
	}
	yard.push_back(std::vector<Object>());
	dogPos.h = objectHeight;
	dogPos.w = objectWidth;

	std::string current;
	std::getline(file, current);

	objectHoriSize = current.size();
	objectVertiSize = 0;

	loadYard(current, objectVertiSize++);
	while (!file.eof())
	{
		yard.push_back(std::vector<Object>());
		std::getline(file, current);
		loadYard(current, objectVertiSize++);
	}
	file.close();
	return checkLvl();
}

//Xử lý events input
void Game::handleUserInput(bool& gameLoop, bool& reverseDir)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			gameLoop = false;
		else if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_RIGHT:
				moveDog(RIGHT);
				reverseDir = false;
				break;
			case SDLK_LEFT:
				moveDog(LEFT);
				reverseDir = true;
				break;
			case SDLK_UP:
				moveDog(UP);
				break;
			case SDLK_DOWN:
				moveDog(DOWN);
				break;
			default:
				break;
			}
		}
	}
}

//Update qua màn mới
void Game::update(bool& gameLoop)
{
	if (sheepCnt == inBarn)
	{
		//Phá đảo game (Pass tất cả levels)

		std::string strLevel = "Level ";
		strLevel += std::to_string(curLvl);
		strLevel.append(" completed");
		initialize.HanldeLevelMsg(strLevel);
		SDL_Delay(2000);

		if (!loadNextLvl())
		{
			initialize.HanldeLevelMsg("YOU WON!");
			SDL_Delay(2000);
			gameLoop = false;
		}

	}
}


//Xóa dữ liệu đếm và dữ liệu mảng yard để cbi đọc file mới
void Game::resetLvlData()
{
	if (!yard.empty())
	{
		for (int i = objectVertiSize - 1; i >= 0; --i)
		{
			for (unsigned int j = yard[i].size(); j > 0; --j)
				yard[i].pop_back();
			yard.pop_back();
		}
	}

	barnCnt = 0;
	sheepCnt = 0;
	inBarn = 0;
	dogCnt = 0;
}

//Load yard từ từng dòng str trong .txt
void Game::loadYard(const std::string line, const unsigned int r)
{
	States curSt;
	bool onBarn = false;
	unsigned int len = line.size();
	if (len > objectHoriSize)
		objectHoriSize = len;
	for (unsigned int i = 0; i < len; ++i)
	{
		switch (line[i])
		{
		case '#':
			curSt = FENCES;
			break;
		case '@':
			curSt = DOGS;
			dogPos.x = objectWidth * i;
			dogPos.y = objectHeight * r;
			++dogCnt;
			break;
		case '+':
			curSt = DOGS;
			onBarn = true;
			dogPos.x = objectWidth * i;
			dogPos.y = objectHeight * r;
			++dogCnt;
			++barnCnt;
			break;
		case '$':
			curSt = SHEEPS;
			++sheepCnt;
			break;
		case '*':
			curSt = SHEEPS;
			onBarn = true;
			++sheepCnt;
			++inBarn;
			++barnCnt;
			break;
		case '.':
			curSt = GRASS;
			onBarn = true;
			++barnCnt;
			break;
		case ' ':
			curSt = GRASS;
			break;
		default:
			curSt = FENCES;
			break;
		}
		yard[r].push_back(Object(r, i, curSt, onBarn));
		onBarn = false;
	}
}

//Sự kiện chuyển màn mới
bool Game::loadNextLvl()
{
	resetLvlData();
	//Ghép str để có được nguồn dẫn vào
	//file .txt của level tiếp theo
	std::string strLevel = "./Project4/Levels/level";
	strLevel += std::to_string(++curLvl);
	strLevel += ".txt";
	//Tự động chuyển lvl nếu không thỏa mãn điều kiện game
	while (curLvl <= lvlTotal && !loadCurLvl(strLevel))
	{
		std::string message = "Level ";
		message += std::to_string(curLvl);
		message += " is invalid.";
		initialize.HanldeLevelMsg(message);
		SDL_Delay(2000);

		resetLvlData();

		strLevel = "level";
		strLevel += std::to_string(++curLvl);
		strLevel += ".txt";
	}
	if (curLvl <= lvlTotal)	
		return true;
	return false;
}

//Hàm lấy tọa độ và di chuyển của chó
void Game::moveDog(Direction direction)
{
	unsigned int r = dogPos.y / objectWidth;
	unsigned int c = dogPos.x / objectHeight;

	moveManage(r, c, direction, DOGS);
}

//Hàm quản lý di chuyển chung
bool Game::moveManage(const unsigned int r, const unsigned int c,
	const Direction direction, const States st)
{
	if (direction == UP)
		moveObject(r, c, -1, 0, st);
	else if (direction == DOWN)
		moveObject(r, c, 1, 0, st);
	else if (direction == LEFT)
		moveObject(r, c, 0, -1, st);
	else if (direction == RIGHT)
		moveObject(r, c, 0, 1, st);

	return true;
}

bool Game::moveObject(const unsigned int r, const unsigned int c, int dX, int dY, const States st)
{	
	
	if (yard[r + dX][c + dY].st == SHEEPS)
		//Chó gặp cừu
	{
		if (moveObject(r + dX, c + dY, dX, dY, SHEEPS))
			yard[r + (dX * 2)][c + (dY * 2)].st = SHEEPS;
		//Cập nhật vị trí cừu nếu có thể di chuyển
		else
			return false;
	}
	//Không thể đẩy 2 con cừu cùng lúc
	if ((yard[r + dX][c + dY].st == FENCES) ||
		(st == SHEEPS && yard[r + dX][c + dY].st == SHEEPS))
		return false;

	
	if (st == DOGS)
	{
		//Di chuyển chó và đặt lại cỏ của vườn
		yard[r + dX][c + dY].st = DOGS;
		dogPos.y += (dX * objectHeight);
		dogPos.x += (dY * objectWidth);
		yard[r][c].st = GRASS;
	}
	//Cập nhật biến 
	else if (st == SHEEPS)
	{
		if (yard[r][c].onBarn)
			--inBarn;
		if (yard[r + dX][c + dY].onBarn)
			++inBarn;
	}
	return true;
}

//Kiểm tra đầu vào của file .txt
//thỏa mãn điều kiện có thể chơi được ?
bool Game::checkLvl() const
{
	if (sheepCnt > barnCnt || !sheepCnt || dogCnt != 1)
		return false;
	return true;
}
