#pragma once
#include "cell.h"
#include <iostream>

class AmazonotGame;
class AmazonotGameStone;

class AmazonotCell:public Cell
{
public:
	/* AmazonotCell constructor */
	AmazonotCell() :Cell(), _isMarked(false), _game(nullptr) {}

	/* AmazonotCell constructor */
	AmazonotCell(AmazonotGame* game, GameStone::Position & position) :Cell(), _game(game), _isMarked(false), _pos(position) {}

	/* AmazonotCell constructor */
	AmazonotCell(GameStone* stone) : Cell(stone){}

	/* AmazonotCell destructor */
	~AmazonotCell() = default;
	
	/* AmazonotCell copy constructor */
	AmazonotCell(const AmazonotCell& other):Cell(other), _isMarked(other._isMarked), _arrowsList(other._arrowsList), _game(other._game){}

	/* AmazonotCell move constructor */
	AmazonotCell(AmazonotCell&& other):Cell(other), _isMarked(other._isMarked), _arrowsList(std::move(other._arrowsList)), _game(other._game) {
		other._game = nullptr;
	}

	/* assignment operator */
	virtual const AmazonotCell& operator=(const AmazonotCell& other);

	/* move assignment operator */
	virtual const AmazonotCell& operator=(AmazonotCell&& other);

	/* the function update isMarked of the cell */
	void setMark(bool mark) { _isMarked = mark; }

	/* the function return true if the cell is markes and false else */
	bool getIsMarked() const { return _isMarked; }

	/* the function set _arrowsList */
	void setArrowsList();

	/* the function return _arrowsList */
	list<GameStone::Position>& getArrowList() { return _arrowsList; }

	/* the function return true if the dst position on _arrowsList and false else */
	bool isOnValidMarksCells(GameStone::Position& src,const GameStone::Position& dst);

	/* the function reset the cell properties */
	virtual void resetCell();

	/* the function set position */
	void setPosition(GameStone::Position& position) { _pos = position; }

	/* the function set game */
	void setGame(AmazonotGame* game) { _game = game; }

	/* the function return list for case 6 on the menu */
	virtual list<GameStone::Position>* getCase6list();

private:
	bool _isMarked;
	list<GameStone::Position> _arrowsList;
	AmazonotGame* _game;
	GameStone::Position _pos;
};

