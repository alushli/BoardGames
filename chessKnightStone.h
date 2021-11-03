#pragma once
#include "chessGameStone.h"

class ChessKnightStone :public ChessGameStone
{
public:
	/* ChessKnightStone constructor */
	ChessKnightStone(istream& in) :ChessGameStone(in) {
		_type = TypeOfStone::KNIGHT;
	}

	/* ChessKnightStone constructor */
	ChessKnightStone(Position& position, int playerNumber, TypeOfStone type, ChessGame* game) : ChessGameStone(position, playerNumber, type, game) {}

	/* ChessKnightStone destructor */
	virtual ~ChessKnightStone() {}

	/* ChessKnightStone copy constructor */
	ChessKnightStone(const ChessKnightStone& other) :ChessGameStone(other) {
		*this = other;
	}

	/* ChessKnightStone move constructor */
	ChessKnightStone(ChessKnightStone&& other) :ChessGameStone(other) {
		*this = std::move(other);
	}

	/* assignment operator */
	virtual const ChessKnightStone& operator= (const ChessKnightStone& other) {
		ChessGameStone::operator=(other);
		return *this;
	}

	/* move assignment operator */
	virtual const ChessKnightStone& operator= (ChessKnightStone&& other) {
		ChessGameStone::operator=(other);
		return *this;
	}

	/* the function create valide moves list for the cur stone */
	virtual void setValidMovesList();

	/* the function get char stone by stone status */
	virtual char getCharByType();

private:
	/* setValidListHelper help function - the function update valids positions of knight */
	void setValidListHelper(Side side2Step, Side side1Step);
};