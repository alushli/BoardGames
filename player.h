#pragma once
#include "gameStone.h"
#include "cell.h"
#include <list>

class Game;

class Player
{
public:
	enum class PlayerStatus {
		WIN, LOSS, NOT_FINISH
	};

	/* Player constructor */
	Player(int playerNumber, Game * game);

	/* Player destructor */
	~Player() = default;

	/* Player copy constructor */
	Player(const Player& other);

	/* Player move constructor */
	Player(Player&& other);

	/* assignment operator */
	const Player& operator=(const Player& other);

	/* move assignment operator */
	const Player& operator=(Player&& other);

	/* the function return stones list as a const */
	const list<GameStone*>& getStonesList()const { return _stoneList; }

	/* the functio retuen player status as a const */
	const PlayerStatus getMyStatus()const { return  _playerStatus; }

	/* the fanction get status and update player status */
	void setStatus(PlayerStatus status) { _playerStatus = status; }

	/* the function get bool value and set isMyTurn of the player */
	void setTurn(bool isMyTurn) { _isMyTurn = isMyTurn; }

	/* the function return player number */
	int getPlayerNumber()const { return _playerNumber; }

	/* the function create stones list for the player */
	void setStoneList();

	/*the function return true if the game end, false otherwise*/
	bool isGameEnd();

	/* the function update opponent */
	void setOpponent(Player* opponent) { _opponent = opponent; }

	/* the function return the stone with pos in stones list */
	GameStone* getStoneFromStonesList(const GameStone::Position& pos);

	/* the function delete the stone fron _stoneList */
	void deletCellFromList(GameStone* stoneToDel);

	/* the function add the stone to _stoneList */
	void addToStoneList(GameStone* toAdd);

private:
	Game* _game;
	Player* _opponent;
	int _playerNumber;
	bool _isMyTurn;
	PlayerStatus _playerStatus;
	list<GameStone*> _stoneList;
};

