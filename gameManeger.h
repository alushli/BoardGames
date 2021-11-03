#pragma once
#include "game.h"
#include "damkaGame.h"
#include"chessGame.h"
#include"amazonotGame.h"
#include <list>
#include <vector>
#include <exception>

/*********************************************************************************************/
/* The class that manages the game. */
/* All inputs and outputs will be performed here, as well as the interaction with the user. */
/*********************************************************************************************/

class GameManeger {
public:

	enum class GamesType
	{
		DAMKA, CHESS, AMAZONOT
	};

	/* Game Maneger constructor */
	GameManeger(){
		if (_listOfGames.empty())
			_cur = _listOfGames.begin();
	}

	/* Game Maneger destructor */
	~GameManeger() {
		deleteGamesList();
	}

	/* Game Maneger copy constructor */
	GameManeger(const GameManeger& other) {
		*this = other;
	}

	/* Game Maneger move constructor */
	GameManeger(GameManeger&& other) {
		*this = std::move(other);
	}

	/* assignment operator */
	const GameManeger& operator= (const GameManeger& other);

	/* move assignment operator */
	const GameManeger& operator=(GameManeger&& other);

	/* the function return the games list as a const */
	const list<Game*>& getGameList()const { return _listOfGames; }

	/* the function return the cur game as a const */
	const Game& getCurGame()const { return **_cur; }

	/*the function print the menu */
	void printMenu();

	/* the function manage the player choose */
	void manageMenu(int choose);

private:
	list<Game*> _listOfGames;
	list<Game*>::iterator _cur;

	/* the function delete the game list */
	void deleteGamesList() {
		for (Game* i : _listOfGames)
			delete i;
	}

	/* the function load new game from file */
	void loadNewGame(string& fName);

	/* the function ask for game type */
	void chooseGameType();
	
	/* the function print the cur game information */
	void printCurGameInfo()const;

	/* the function print the board of the cur game */
	void printCurBoard()const;

	/* the function execute case 5 or 6 from the menu */
	void case5Or6(int choose)const;

	/* the function print valid moves list of the stone in the get position */
	void printValidMovesList(GameStone::Position& position)const;

	/* the function print risk list of the stone in the get position */
	void printRiskList(list<GameStone::Position>* list)const;

	/* the function execute case 7 from the menu */
	void case7();

	/* the function make user move on the cur game and update the cur player */
	void makeMove(GameStone::Position * positions);

	/* the function save the game to file */
	void saveGame(string& fName);

	/* the function request file name */
	void getFileName(int case9or10);

	/* the function delete the game */
	void badAllocHandle(bad_alloc& ex);

	/* the function update _cur to the next game */
	void nextGame();

	/* the function delete the cur game */
	void deleteCurGame();
};