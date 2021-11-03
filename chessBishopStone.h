#pragma once
#include "chessGameStone.h"

class ChessBishopStone :public ChessGameStone
{
public:
	/* ChessBishopStone constructor */
	ChessBishopStone(istream& in) :ChessGameStone(in) {
		_type = TypeOfStone::BISHOP;
	}

	/* ChessBishopStone constructor */
	ChessBishopStone(Position& position, int playerNumber, TypeOfStone type, ChessGame* game) : ChessGameStone(position, playerNumber, type, game) {}

	/* ChessBishopStone destructor */
	virtual ~ChessBishopStone(){}

	/* ChessBishopStone copy constructor */
	ChessBishopStone(const ChessBishopStone& other) :ChessGameStone(other) {
		*this = other;
	}

	/* ChessBishopStone move constructor */
	ChessBishopStone(ChessBishopStone&& other) :ChessGameStone(other) {
		*this = other;
	}

	/* assignment operator */
	virtual const ChessBishopStone& operator= (const ChessBishopStone& other) {
		ChessGameStone::operator=(other);
		return *this;
	}

	/* move assignment operator */
	virtual const ChessBishopStone& operator= (ChessBishopStone&& other) {
		ChessGameStone::operator=(other);
		return *this;
	}

	/* the function create valide moves list for the cur stone */
	virtual void setValidMovesList();

	/* the function get char stone by stone status */
	virtual char getCharByType();

private:
	/* setValidListHelper help function - the function update valids positions of bishop */
	void setValidListHelper(Side sideRow, Side sideCol);
};