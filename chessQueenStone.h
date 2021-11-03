#pragma once
#include "chessGameStone.h"

class ChessQueenStone :public ChessGameStone
{
public:
	/* ChessQueenStone constructor */
	ChessQueenStone(istream& in) :ChessGameStone(in) {
		_type = TypeOfStone::QUEEN;
	}

	/* ChessQueenStone constructor */
	ChessQueenStone(Position& position, int playerNumber, TypeOfStone type, ChessGame* game) : ChessGameStone(position, playerNumber, type, game) {}

	/* ChessQueenStone destructor */
	virtual ~ChessQueenStone(){}

	/* ChessQueenStone copy constructor */
	ChessQueenStone(const ChessQueenStone& other) :ChessGameStone(other) {
		*this = other;
	}

	/* ChessQueenStone move constructor */
	ChessQueenStone(ChessQueenStone&& other) :ChessGameStone(other) {
		*this = std::move(other);
	}

	/* assignment operator */
	virtual const ChessQueenStone& operator= (const ChessQueenStone& other) {
		ChessGameStone::operator=(other);
		return *this;
	}

	/* move assignment operator */
	virtual const ChessQueenStone& operator= (ChessQueenStone&& other) {
		ChessGameStone::operator=(other);
		return *this;
	}

	/* the function create valide moves list for the cur stone */
	virtual void setValidMovesList();

	/* the function get char stone by stone status */
	virtual char getCharByType();

private:
	/* setValidListHelper help function - the function update valids positions of queen */
	void setValidListHelper(Side sideRow, Side sideCol);
};