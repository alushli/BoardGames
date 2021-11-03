#pragma once
#include "chessGameStone.h"

class ChessPawnStone :public ChessGameStone
{
public:
	static const int rowPlayer1 = 7;
	static const int rowPlayer2 = 2;

	/* ChessPawnStone constructor */
	ChessPawnStone(istream& in) :ChessGameStone(in) {
		_type = TypeOfStone::PAWN;
	}

	/* ChessPawnStone constructor */
	ChessPawnStone(Position& position, int playerNumber, TypeOfStone type, ChessGame* game) : ChessGameStone(position, playerNumber, type, game) {}

	/* ChessPawnStone destructor */
	virtual ~ChessPawnStone(){}

	/* ChessPawnStone copy constructor */
	ChessPawnStone(const ChessPawnStone& other) :ChessGameStone(other) {
		*this = other;
	}

	/* ChessPawnStone move constructor */
	ChessPawnStone(ChessPawnStone&& other) :ChessGameStone(other) {
		*this = std::move(other);
	}

	/* assignment operator */
	virtual const ChessPawnStone& operator= (const ChessPawnStone& other) {
		ChessGameStone::operator=(other);
		return *this;
	}

	/* move assignment operator */
	virtual const ChessPawnStone& operator= (ChessPawnStone&& other) {
		ChessGameStone::operator=(other);
		return *this;
	}

	/* the function create valide moves list for the cur stone */
	virtual void setValidMovesList();

	/* the function get char stone by stone status */
	virtual char getCharByType();

private:
	/* setValidListHelper help function - the function update valids positions of pawn */
	void setValidListHelper(Side sideRow, Side sideCol, int stepNum);
};