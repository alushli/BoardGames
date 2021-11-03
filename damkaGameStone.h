#pragma once
#include "gameStone.h"
#include"damkaGame.h"

class DamkaGameStone:public GameStone
{
public:
	static const int end_board_rows_player_1 = 3;
	static const int start_board_rows_player_2 = 6;
	enum class TypeOfStone
	{
		KING , REGULAR
	};

	/* DamkaGameStone constructor */
	DamkaGameStone(istream& in) :GameStone(in), _type(TypeOfStone::REGULAR) {}

	/* DamkaGameStone constructor */
	DamkaGameStone(DamkaGame* game, int playarNumber, Position _position);

	/* DamkaGameStone destructor */
	~DamkaGameStone();

	/* DamkaGameStone copy constructor */
	DamkaGameStone(const DamkaGameStone& other);

	/* DamkaGameStone move constructor */
	DamkaGameStone(DamkaGameStone&& other) :GameStone(other) {
		*this = std::move(other);
	}

	/* assignment operator */
	const DamkaGameStone& operator= (const DamkaGameStone& other);

	/* move assignment operator */
	const DamkaGameStone& operator=(DamkaGameStone&& other);

	/* the function return true if the position on the valid moves list or false else */
	virtual bool isInMovesList( Position& position)const;

	/* the function return valid moves list of the stone*/
	const list<GameStone::Position>& getValidMovesList()const { return _validMovesList; }

	/* the function create valide moves list for the cur stone */
	virtual void setValidMovesList();

	/* the function return risk list of the stone */
	virtual list<GameStone::Position>* getRiskList() { return &_riskList; }

	/* the function set risk list to the stone */
	void setRiskList();

	/* the function return stone type */
	TypeOfStone getType()  { return _type; }

	/* the function get char stone by stone status */
	virtual char getCharByType();

	/* the function update stone type */
	void setStatus(TypeOfStone type) { _type = type; }

	/* the function update game property */
	void setGame(DamkaGame* game) { _game = game; }

	/* the function save the game to file */
	virtual void save(ostream& outFile) const;

private:
	DamkaGame* _game;
	list<GameStone::Position>  _validMovesList;
	list<GameStone::Position>  _riskList;
	TypeOfStone _type;

	/* the function find the stones that mack risk on the cur stone and update risks list */
	void findRiskBySlantAndByDirection(Side direction, Side slant);
	
	/* the function update the position by slant and direction */
	/* this is helper function to findRiskBySlantAndByDirection */
	void updatePositionInLoopHelper(Position& position, Side slant, Side direction)const;
	
	/* the function update the position to the position of the back position hypotenuse by slant and direction*/
	void setBackPositionBySlantAndDirection(Position& position, Side slant, Side direction)const;
	
	/* the function update the moves list when the stone can move 1 step - REGULAR with no eating */
	void checkOneStep(const Position& position, Side side);
	
	/* the function update the moves list when the stone can move 1 step - REGULAR with no eating */
	void checkTwoStep(const Position& posiotion, Side side);
	
	/* the function update the moves list when the stone is KING */
	void checkKingStep(const Position& posiotion, Side sideY, Side sideX);
	
	/* the function update the position according to the player and move of the stone to sideX and sideY */
	void setPointStepAccordingPlayer(Position& posiotion, Side sideY, Side sideX);
};

