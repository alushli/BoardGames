#pragma once
#include "gameStone.h"
#include "amazonotCell.h"
#include "amazonotGame.h"
#include <list>

class AmazonotGameStone:public GameStone
{
public:
	/* AmazonotGameStone constructor */
	AmazonotGameStone(istream& in) :GameStone(in) {}

	/* AmazonotGameStone constructor */
	AmazonotGameStone(AmazonotGame* game,int playerNumber, Position position) :GameStone(playerNumber, position), _game(game){}

	/* AmazonotGameStone destructor */
	virtual ~AmazonotGameStone();

	/* AmazonotGameStone copy constructor */
	AmazonotGameStone(const AmazonotGameStone& other):GameStone(other) {
		*this = other;
	}

	/* AmazonotGameStone move constructor */
	AmazonotGameStone(AmazonotGameStone&& other):GameStone(other) {
		*this = other;
	}

	/* assignment operator */
	const AmazonotGameStone& operator= (const AmazonotGameStone& other);

	/* move assignment operator */
	const AmazonotGameStone& operator=(AmazonotGameStone&& other);

	/* the function return true if the position on the valid moves list and false else */
	virtual bool isInMovesList( Position& position)const;
	
	/* the function return valid moves list of the stone */
	virtual const list<GameStone::Position>& getValidMovesList()const { return _validMovesList; }

	/* the function return valid moves list of the stone */
	list<GameStone::Position>& getValidMovesList() { return _validMovesList; }

	/* the function create valide moves list for the cur stone */
	virtual void setValidMovesList();

	/* the function get char stone by stone status */
	virtual char getCharByType();

	/* the function update the game property */
	void setGame(AmazonotGame* game) { _game = game; }
	
private:
	AmazonotGame* _game;
	list<GameStone::Position>  _validMovesList;

	/* setValidMovesList help function - the function check valid posions and add to  validMovesList positions */
	void setValidListHelper(Side upOrDown, Side leftOrRight);

	/* setValidMovesList help function - the function update pos indexs */
	void PromotePosIndex(Side upOrDown, Side leftOrRight, Position& pos);
};

