#pragma once
#include "chessGameStone.h"


class ChessKingStone :public ChessGameStone
{
public:
	/* ChessKingStone constructor */
	ChessKingStone(istream& in):ChessGameStone(in){
		_type = TypeOfStone::KING;
	}

	/* ChessKingStone constructor */
	ChessKingStone(Position& position, int playerNumber, TypeOfStone type, ChessGame* game): ChessGameStone(position, playerNumber, type, game) {}

	/* ChessKingStone destructor */
	virtual ~ChessKingStone(){}

	/* ChessKingStone copy constructor */
	ChessKingStone(const ChessKingStone& other) :ChessGameStone(other) {
		*this = other;
	}

	/* ChessKingStone move constructor */
	ChessKingStone(ChessKingStone&& other) :ChessGameStone(other) {
		*this = std::move(other);
	}

	/* assignment operator */
	virtual const ChessKingStone& operator= (const ChessKingStone& other) {
		ChessGameStone::operator=(other);
		return *this;
	}

	/* move assignment operator */
	virtual const ChessKingStone& operator= (ChessKingStone&& other) {
		ChessGameStone::operator=(other);
		return *this;
	}

	/* the function create valide moves list for the cur stone */
	virtual void setValidMovesList();

	/* the function get char stone by stone status */
	virtual char getCharByType();

private:

	/* setValidListHelper help function - the function update valids positions of king */
	void setValidListHelper(Side sideRow, Side sideCol);
};