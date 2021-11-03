#pragma once
#include "gameStone.h"
#include "chessGame.h"
#include <iostream>
class ChessGameStone:public GameStone
{
public:
	enum class TypeOfStone
	{
		KING, ROOK, BISHOP, QUEEN, KNIGHT, PAWN
	};

	/* ChessGameStone constructor */
	ChessGameStone(istream& in) :GameStone(in) {}
	
	/* ChessGameStone constructor */
	ChessGameStone(Position& position, int playerNumber, TypeOfStone type, ChessGame* game) :GameStone(playerNumber, position), _type(type), _game(game) {}

	/* ChessGameStone destructor */
	virtual ~ChessGameStone();

	/* ChessGameStone copy constructor */
	ChessGameStone(const ChessGameStone& other);

	/* ChessGameStone move constructor */
	ChessGameStone(ChessGameStone&& other) :GameStone(other) {
		*this = std::move(other);
	}

	/* assignment operator */
	virtual const ChessGameStone& operator=(const ChessGameStone& other);

	/* move assignment operator */
	virtual const ChessGameStone& operator=(ChessGameStone&& other);

	/* the function return true if the position on the valid moves list or false else */
	virtual bool isInMovesList(Position& position)const;

	/* the function return valid moves list of stone */
	virtual const list<GameStone::Position>& getValidMovesList()const { return _validMovesList; }

	/* the function create valide moves list for the cur stone */
	virtual void setValidMovesList() = 0;

	/* the function set risk list to the stone */
	void setRiskList();

	/* the function return stone type */
	TypeOfStone getType() const { return _type; }

	/* the function get char stone by stone status */
	virtual char getCharByType() = 0;

	/* the function return risk list */
	virtual list<GameStone::Position>* getRiskList() { return &_riskList; }

	/* the function update game property */
	void setGame(ChessGame* game) { _game = game; }

	/* the function save the game to file */
	virtual void save(ostream& outFile) const;

protected:
	ChessGame* _game;
	list<GameStone::Position>  _validMovesList;
	list<GameStone::Position>  _riskList;
	TypeOfStone _type;

	/* setValidMovesList help function - the function update pos indexs */
	void PromotePosIndex(Side sideRow, Side sideCol, Position& pos);

	/* the function add position to valid moves list */
	void addPositionToList(Position& position);

	/* the function add stone to risk list */
	void addStoneToList(ChessGameStone* stone);

	/* setRiskList help function - the function update risk of stones */
	void setRiskListHelper(Side sideRow, Side sideCol);

	/* setRiskList help function - the function update risk of knight stones */
	void chechRisksKnightOfOpponent();

	/* the function return true if the stone can be in the cell board according to stone type and false else */
	bool canBeThere(TypeOfStone type, Side sideRow, Side sideCol, int stepCount);

	/* the function return true if the knight on the cell and false else */
	void isKnightThere(Side side2Step, Side side1Step);
};

