#include "game.h"
#define rcastc reinterpret_cast<char*>
#define rcastcc reinterpret_cast<const char*>

/* Game constructor */
Game::Game(istream& in):_player1(1,this), _player2(2, this), _winner(nullptr), _curPlayerTurn(nullptr){
	in.read(rcastc(&_isOnGoing), sizeof(_isOnGoing));
	if (!in.good())
		throw FileException("Fail in read from file in game file constructor");
	int curPlayerNumber, winnerNumber;
	in.read(rcastc(&winnerNumber), sizeof(winnerNumber));
	if (!in.good())
		throw FileException("Fail in read from file in game file constructor");
	
	in.read(rcastc(&curPlayerNumber), sizeof(curPlayerNumber));
	if (!in.good())
		throw FileException("Fail in read from file in game file constructor");
	try {
		setCurPlayer(curPlayerNumber);
		setWinner(winnerNumber);

		if (winnerNumber == 1 || winnerNumber == 2) {
			if (winnerNumber == 1) {
				_player1.setStatus(Player::PlayerStatus::WIN);
				_player2.setStatus(Player::PlayerStatus::LOSS);
			}
			else {
				_player2.setStatus(Player::PlayerStatus::WIN);
				_player1.setStatus(Player::PlayerStatus::LOSS);
			}
		}
	}
	catch (FileException& ex) {
		throw;
	}
	catch (invalid_argument& ex) {
		throw;
	}
}

/* Game constructor */
Game::Game():_winner(nullptr), _player1(1, this), _player2(2, this), _isOnGoing(true), _curPlayerTurn(&_player1) {
	_player1.setOpponent(&_player2);
	_player2.setOpponent(&_player1);
}

/* Game copy constructor */
Game::Game(const Game& other) :_player1(other._player1), _player2(other._player2), _isOnGoing(other._isOnGoing),
_winner(other._winner), _curPlayerTurn(other._curPlayerTurn), _requirePositions(other._requirePositions){}

/* assignment operator */
const Game& Game::operator= (const Game& other) {
	if (this != &other) {
		_requirePositions = other._requirePositions;
		_player1 = other._player1;
		_player2 = other._player2;
		_isOnGoing = other._isOnGoing;
		_winner = other._winner;
		_curPlayerTurn = other._curPlayerTurn;
	}
	return *this;
}

/* the function get number of player and return the relevant player */
Player& Game::getPlayer(int numOfPlayer) {
	if (numOfPlayer == 1)
		return _player1;
	else if (numOfPlayer == 2)
		return _player2;
	else
		throw invalid_argument("Invalid arg for numOfPlayer");
}

/* the function get number of player and update the winner of the game to be this player */
void Game::setWinner(int numOfPlayer) {
	if (numOfPlayer == 1)
		_winner = &_player1;
	else if (numOfPlayer == 2)
		_winner = &_player2;
	else if (numOfPlayer != 0)
		throw invalid_argument("Invalid arg for numOfPlayer");
}

/* the function get number of player and update the cur player to be this player */
void Game::setCurPlayer(int numOfPlayer) {
	if (numOfPlayer == 1) {
		_curPlayerTurn = &_player1;
		_player1.setTurn(true);
		_player2.setTurn(false);
	}
	
	else if (numOfPlayer == 2) {
		_curPlayerTurn = &_player2;
		_player1.setTurn(false);
		_player2.setTurn(true);
	}
		
	else if(numOfPlayer != 0)
		throw invalid_argument("Invalid arg for numOfPlayer");
}

/* the function save the game to file */
void Game::save(ostream & outFile) const {
	int winnerNum = 0, curPlayerTurn = 0;
	if (_winner)
		winnerNum = _winner->getPlayerNumber();
	if (_curPlayerTurn)
		curPlayerTurn = _curPlayerTurn->getPlayerNumber();

	outFile.write(rcastcc(&_isOnGoing), sizeof(_isOnGoing));
	if (!outFile.good())
		throw FileException("Fail in read from file - method save");
	outFile.write(rcastcc(&winnerNum), sizeof(winnerNum));
	if (!outFile.good())
		throw FileException("Fail in read from file - method save");
	outFile.write(rcastcc(&curPlayerTurn), sizeof(curPlayerTurn));
	if (!outFile.good())
		throw FileException("Fail in read from file - method save");
}

/* the function save stones list of player to file */
void Game::saveStonesListOfPlayer(ostream & outFile, int playerNum) const {
	list<GameStone*> list;
	if (playerNum != 1 && playerNum != 2)
		throw invalid_argument("Invalid arg in saveStonesListOfPlayer method");
	playerNum == 1 ? list = _player1.getStonesList() : list = _player2.getStonesList();

	for (auto itr = list.begin(); itr != list.end(); ++itr) {
		(*itr)->save(outFile);
	}
}


