#pragma once
#include "game.h"
#include "amazonotCell.h"
#include <fstream>
#include <vector>
using namespace std;

class AmazonotGame:public Game
{
public:
	static const int amazonot_size = 10;
	static const int amazonot_stones_number = 8;

	/* AmazonotGame constructor */
	AmazonotGame();

	/* AmazonotGame constructor */
	AmazonotGame(const AmazonotGame& other) :Game(other) {
		*this = other;
	}

	/* AmazonotGame constructor */
	AmazonotGame(AmazonotGame&& other) :Game(other) {
		*this = other;
	}

	/* AmazonotGame constructor */
	AmazonotGame(istream& in);

	/* AmazonotGame destructor */
	virtual ~AmazonotGame();

	/* assignment operator */
	virtual const AmazonotGame& operator= (const AmazonotGame& other);

	/* move assignment operator */
	virtual const AmazonotGame& operator= (AmazonotGame && other);

	/* the function make user move on the game and return move status */
	virtual void makeMove(GameStone::Position& src, GameStone::Position& dstMove, GameStone::Position* dstArrow);

	/* the function return the cell on board game */
	virtual Cell& getCellOnBoard(const GameStone::Position& position);

	/* the function return true if position on board lines and false else */
	virtual bool isInBoardLines(const GameStone::Position& position)const;

	/* the function return the board size */
	virtual int getBoardSize() { return amazonot_size; };

	/* the function return true if game end and false else */
	virtual bool isGameEnd();

	/* the function save the game to file */
	virtual void save(ostream& outFile) const;
	

private:
	AmazonotCell _board[amazonot_size][amazonot_size];
	int _countMarkCells;

	/* the function set the game*/
	void setBoard();

	/* the function delete all the stones on the board */
	void deleteAllStones();

	/* the function set stones on board */
	void setStoneOnBoard(const GameStone::Position& dst, GameStone* stone, const GameStone::Position& arrow);
	
	/* the function save the marked cells to file */
	void saveMarkedCells(ostream & outFile) const;

	/* the function set the board according to the file */
	void setBoardFromFile(vector<AmazonotGameStone*>& vectorOfStones, vector<GameStone::Position>& vectorOfPositions);

	/* the function delete all vector data */
	void deletVectorData(vector<AmazonotGameStone*>& v);
};

