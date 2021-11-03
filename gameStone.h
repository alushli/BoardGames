#pragma once
#include <iostream>
#include <fstream>
#include <list>
using namespace std;
#include "fileExceptions.h"
#define rcastc reinterpret_cast<char*>

class GameStone
{
public:
	enum class Side
	{
		LEFT, RIGTH, FORWARD, BACKWARD, NONE
	};

	struct Position
	{
		int x; //row
		int y; //col

		/* operator cin to position */
		friend istream& operator>>(istream& is, GameStone::Position& position);

		/* operator cout to print the position */
		friend ostream& operator<<(ostream& os, GameStone& position);

		/* comparison operator of positions */
		bool operator==(const Position& other) const{
			return x == other.x && y == other.y;
		}

		/* assignment operator of positions */
		const Position& operator=(const Position& other) {
			x = other.x;
			y = other.y;
			return *this;
		}

		/* the function save the position to file */
		void save(ostream& outFile) const;
	};

public:

	/* Game Stone constructor */
	GameStone( int playerNumber, Position & position): _playerNumber(playerNumber),_position(position){
		if (playerNumber != 1 && playerNumber != 2)
			throw invalid_argument("Invalid playerNum");
	}

	/* Game Stone destructor */
	virtual ~GameStone() = default;

	/* Game Stone copy constructor */
	GameStone(const GameStone& other) {
		*this = other;
	}

	/* assignment operator */
	const GameStone& operator= (const GameStone& other);

	/* the function return the position of the stone as a const */
	const Position& getPosition()const { return  _position; }

	/* the function return valid moves list of the stone as a const */
	virtual const list<GameStone::Position>& getValidMovesList()const = 0;

	/* the function get position update stone position */
	void setPosition(const Position& position) { _position = position; }

	/* the function return true if the position on the valid moves list or false else */
	virtual bool isInMovesList(Position& position)const = 0;

	/* the function create valide moves list for the cur stone */
	virtual void setValidMovesList() = 0;

	/* the function set player number */
	void setPlayer(int player) { _playerNumber = player; }

	/* the funtion return player number */
	int getPlayerNum()const { return _playerNumber; }

	/* operator cout to print the cell*/
	friend ostream& operator<<(ostream& os, GameStone& stone);

	/* the function get char stone by stone status */
	virtual char getCharByType() =0;

	/* the function return risk list */
	virtual list<GameStone::Position>* getRiskList() { return nullptr; }

	/* the function save the game to file */
	virtual void save(ostream& outFile) const;
	
protected:
	int _playerNumber;
	Position _position;

	/* Game Stone constructor */
	GameStone(istream& in);
};

