#pragma once
#include "chessGameStone.h"

class ChessRookStone :public ChessGameStone
{
public:
	/* ChessRookStone constructor */
	ChessRookStone(istream& in) :ChessGameStone(in) {
		_type = TypeOfStone::ROOK;
	}

	/* ChessRookStone constructor */
	ChessRookStone(Position& position, int playerNumber, TypeOfStone type, ChessGame* game) : ChessGameStone(position, playerNumber, type, game) {}

	/* ChessRookStone destructor */
	virtual ~ChessRookStone(){}

	/* ChessRookStone copy constructor */
	ChessRookStone(const ChessRookStone& other) :ChessGameStone(other) {
		*this = other;
	}

	/* ChessRookStone move constructor */
	ChessRookStone(ChessRookStone&& other) :ChessGameStone(other) {
		*this = std::move(other);
	}

	/* assignment operator */
	virtual const ChessRookStone& operator= (const ChessRookStone& other) {
		ChessGameStone::operator=(other);
		return *this;
	}

	/* move assignment operator */
	virtual const ChessRookStone& operator= (ChessRookStone&& other) {
		ChessGameStone::operator=(other);
		return *this;
	}

	/* the function create valide moves list for the cur stone */
	virtual void setValidMovesList();

	/* the function get char stone by stone status */
	virtual char getCharByType();

private:
	/* setValidListHelper help function - the function update valids positions of rook */
	void setValidListHelper(Side sideRow, Side sideCol);
};