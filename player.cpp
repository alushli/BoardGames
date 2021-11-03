#include "player.h"
#include "game.h"
#include <iostream>

/* Player constructor */
Player::Player( int playerNumber, Game * game) : _playerNumber(playerNumber), _playerStatus(PlayerStatus::NOT_FINISH), _opponent(nullptr), _game(game) {
	if (playerNumber == 1)
		_isMyTurn = true;
	else
		_isMyTurn = false;
}

/* Player copy constructor */
Player::Player(const Player& other) {
	*this = other;
}

/* Player move constructor */
Player::Player(Player&& other) {
	*this = other;
}

/* assignment operator */
const Player& Player::operator=(const Player& other) {
	if (this != &other) {
		_stoneList = other._stoneList;
		_isMyTurn = other._isMyTurn;
		_playerStatus = other._playerStatus;
		_playerNumber = other._playerNumber;
		_opponent = other._opponent;
		_game = other._game;
	}
	return *this;
}

/* move assignment operator */
const Player& Player::operator=(Player&& other) {
	if (this != &other) {
		_stoneList = std::move(other._stoneList);
		_isMyTurn = other._isMyTurn;
		_playerStatus = other._playerStatus;
		_playerNumber = other._playerNumber;
		_opponent = other._opponent;
		other._opponent = nullptr;
		_game = other._game;
		other._game = nullptr;
	}
	return *this;
}

/* the function create stones list for the player */
void Player::setStoneList() {
	int size = _game->getBoardSize();
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++) {
			GameStone::Position pos = { i + 1, j + 1 };
			if (_game->getCellOnBoard(pos).getStone() && _game->getCellOnBoard(pos).getStone()->getPlayerNum() == _playerNumber)
				_stoneList.push_back((_game->getCellOnBoard(pos).getStone()));
		}
	}
}

/*the function return true if the game end, false otherwise*/
bool Player::isGameEnd() {
	for (auto itr = _stoneList.begin(); itr != _stoneList.end(); ++itr) {
		if (!((*itr)->getValidMovesList().empty())) 
			return false;
	}
	return true;
}

/* the function return the stone with pos in stones list */
GameStone* Player::getStoneFromStonesList(const GameStone::Position& pos) {
	for (auto itr = _stoneList.begin(); itr != _stoneList.end(); ++itr) {
		if ((*itr)->getPosition() == pos)
			return *(itr);
	}
	return nullptr;
}

/* the function delete the stone fron _stoneList */
void Player::deletCellFromList(GameStone* stoneToDel) {
	for (auto itr = _stoneList.begin(); itr != _stoneList.end(); ++itr)
	{
		if (*itr == stoneToDel) {
			if (itr == --_stoneList.end()) {
				_stoneList.erase(itr);
				itr = --_stoneList.end();
			}
			else
				itr = _stoneList.erase(itr);
			delete stoneToDel;
		}		
	}
}

/* the function add the stone to _stoneList */
void Player::addToStoneList(GameStone * toAdd) {
	_stoneList.push_back(toAdd);
}


