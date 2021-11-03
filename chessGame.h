#pragma once
#include "game.h"
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

class ChessGameStone;
class ChessKingStone;

class ChessGame : public Game
{
public:
	static const int chess_size = 8;
	static const int chess_stones_number = 32;

	/* ChessGame constructor */
	ChessGame();

	/* ChessGame constructor */
	ChessGame(const ChessGame& other);

	/* ChessGame destructor */
	virtual ~ChessGame();

	/* ChessGame constructor */
	ChessGame(istream& in);

	/* ChessGame copy constructor */
	ChessGame(ChessGame& other);

	/* ChessGame move constructor */
	ChessGame(ChessGame&& other):Game(move(other)) { *this = move(other); }

	/* assignment operator */
	virtual const ChessGame& operator= (const ChessGame& other);

	/* move assignment operator */
	virtual const ChessGame& operator= (ChessGame&& other);

	/* the function make user move on the game and return move status */
	virtual void makeMove(GameStone::Position& src, GameStone::Position& dstMove, GameStone::Position* dstArrow);
	
	/* the function return cell board according to the position */
	virtual Cell& getCellOnBoard(const GameStone::Position& position);
	
	/* the function return true if the position on board lines and false else */
	virtual bool isInBoardLines(const GameStone::Position& position)const {
		return position.x > 0 && position.x <= chess_size && position.y > 0 && position.y <= chess_size;
	}
	
	/* the function return board size */
	virtual int getBoardSize() { return chess_size; }

	/* the function return true if game end and false else */
	virtual bool isGameEnd();

	/* the function return true if the move danger the king and false else */
	bool isDangerKing(ChessGameStone * stone, const GameStone:: Position& src, const GameStone::Position& dst);

	/* the function save the game to file */
	virtual void save(ostream& outFile) const;

private:
	Cell _board[chess_size][chess_size];
	ChessKingStone* _king1, *_king2;
	int _countEatenStones;

	/* the function set the board */
	void setBoard();

	/* the function set dstMove cell on board */
	void setCellOnBoard(GameStone::Position & dstMove, GameStone * stone);

	/* the function set the first row of the game for all players */
	void setFirstRow(int playerNum);

	/* the function set the second row of the game for all players */
	void setSecondRow(int playerNum);

	/* the function set valid move list for all stones */
	void setValidMovesToStones();

	/* the function set risk list for all stones */
	void setRisksToStones();

	/* the function return new stone by type */
	ChessGameStone* allocNewChessStoneByType(ChessGameStone* stone);

	/* the function set board game from file */
	void setBoardFromFile(vector<ChessGameStone*> & v);

	/* the function delete all vector date */
	void deletVectorData(vector<ChessGameStone*>& v);

	/* the function delete all the stones on board */
	void deleteAllStones();
};
