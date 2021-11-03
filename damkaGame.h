#pragma once
#include "game.h"
#include <vector>
using namespace std;

class DamkaGameStone;

class DamkaGame : public Game
{
public:
	static const int damka_size = 8;
	static const int  damka_stones_number = 24;

	/* DamkaGame constructor */
	DamkaGame(istream& in);

	/* DamkaGame constructor */
	DamkaGame();
	
	/* DamkaGame copy constructor  */
	DamkaGame(const DamkaGame& other);

	/* DamkaGame move constructor  */
	DamkaGame(DamkaGame&& other) :Game(other) {
		*this = std::move(other);
	}
	
	/* DamkaGame destructor */
	virtual ~DamkaGame();

	/* assignment operator */
	virtual const DamkaGame& operator= (const DamkaGame& other);

	/* move assignment operator */
	virtual const DamkaGame& operator= (DamkaGame&& other);

	/* the function make user move on the game and return move status */
	virtual void makeMove(GameStone::Position& src, GameStone::Position& dstMove, GameStone::Position* dstArrow);
	
	/* the function return cell board according to the position */
	virtual Cell& getCellOnBoard(const GameStone::Position& position);
	
	/* the function return true if the position on board lines and false else */
	virtual bool isInBoardLines(const GameStone::Position& position)const {
		return position.x > 0 && position.x <= damka_size && position.y > 0 && position.y <= damka_size;
	}

	/* the function return board size */
	virtual int getBoardSize() { return damka_size; }

	/* the function return true if game end and false else */
	virtual bool isGameEnd();

	/* the function save the game to file */
	virtual void save(ostream& outFile) const;

private:
	Cell _board[damka_size][damka_size];
	int _countEatenStones;

	/* the function return the eaten stone of the move */
	GameStone* getEatenStone(const GameStone& stone, const GameStone::Position& dst);
	
	/* the function calculates the difference in places between the stones and return the position of the difference */
	GameStone::Position getMinusPos(const GameStone::Position& src, const GameStone::Position& dst)const;

	/* the function return the postion that between src and dst */
	/* if src of the stone is KING, return the position that before the dst on hypotenuse */
	GameStone::Position getPrevPos(const GameStone::Position& src, const GameStone::Position& dst)const;
	
	/* the function set dstMove cell on board */
	void setCellOnBoard(GameStone::Position& dst, GameStone* stone);

	/* the function set the board according to file */
	void setBoardFromFile(vector<DamkaGameStone*> &v);

	/* the function set risk list and valid list to all stones */
	void setRiskAndValidAllStones();

	/* the function delete vector data */
	void deletVectorData(vector<DamkaGameStone*>& v);

	/* the function delete all sotones on the board */
	void deleteAllStones();
};


