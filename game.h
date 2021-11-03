#pragma once
#include "player.h"
#include <iostream>
#include <fstream>
#include "fileExceptions.h"

class Game
{
public:
	/* Game constructor */
	Game();

	/* Game destructor */
	virtual ~Game() = default;

	/* Game copy constructor */
	Game(const Game& other);

	/* assignment operator */
	virtual const Game& operator= (const Game & other);

	/* the function return the winner player as a const */
	const Player* getWinner() const { return _winner; }

	/* the function return the cur player */
	Player* getCurPlayer() const { return _curPlayerTurn; }

	/* the function get number of player and return the relevant player */
	Player& getPlayer(int numOfPlayer);

	/* the function get number of player and update the winner of the game to be this player */
	void setWinner(int numOfPlayer);

	/* the function get number of player and update the cur player to be this player */
	void setCurPlayer(int numOfPlayer);

	/* the function get status and update the game status */
	void setGameStatus(bool status) { _isOnGoing = status; }

	/* the function return game status */
	bool getGameStatus() { return _isOnGoing; }

	/* the function make user move on the game and return move status */
	virtual void makeMove(GameStone::Position& src, GameStone::Position& dstMove, GameStone::Position* dstArrow) = 0;

	/* the function return cell board according to the position */
	virtual Cell& getCellOnBoard(const GameStone::Position& position) = 0;

	/* the function return true if the position on board lines and false else */
	virtual bool isInBoardLines(const GameStone::Position& position)const = 0;

	/* the function return board size */
	virtual int getBoardSize() = 0;

	/* the function return true if game end and false else */
	virtual bool isGameEnd() = 0;

	/* operator cout to print the board */
	friend ostream& operator<<(ostream& os, Game* game);

	/* the function update _requirePositions */
	int getRequirePositions() { return _requirePositions; }

	/* the function save the game to file */
	virtual void save(ostream & outFile) const;

protected:
	Player _player1, _player2;
	bool _isOnGoing;
	Player* _winner;
	Player* _curPlayerTurn;
	int _requirePositions;

	/* Game constructor */
	Game(istream& in);

	/* the function save stones list of player to file */
	void saveStonesListOfPlayer(ostream & outFile, int playerNum) const;
};

