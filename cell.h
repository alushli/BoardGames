#pragma once
#include "gameStone.h"
#include <iostream>
#include <list>
using namespace std;

class Cell
{
public:
	/* Cell constructor */
	Cell() :_stone(nullptr){}

	/* Cell constructor */
	Cell(GameStone* stone) :_stone(stone){}

	/* Cell destructor */
	virtual ~Cell() = default;

	/* Cell copy constructor */
	Cell(const Cell& other) {
		*this = other;
	}

	/* Cell move constructor */
	Cell(Cell&& other) {
		*this = other;
	}

	/* assignment operator */
	virtual const Cell& operator=(const Cell& other);

	/* move assignment operator */
	virtual const Cell& operator=(Cell&& other);

	/* the function return the stone on the cell */
	GameStone* getStone()const { return _stone; }

	/* the function update stone of the cell */
	void setStone(GameStone* stone) { _stone = stone; }

	/* operator cout to print the cell*/
	friend ostream& operator<<(ostream& os, Cell& cell);

	/* the function reset the cell properties */
	virtual void resetCell() { _stone = nullptr; }

	/* the function return list for case 6 on the menu */
	virtual list<GameStone::Position>* getCase6list();

protected:
	GameStone* _stone;
};

