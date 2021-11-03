#include "amazonotCell.h"
#include "amazonotGameStone.h"
#include <iostream>

/* assignment operator */
const AmazonotCell& AmazonotCell::operator=(const AmazonotCell & other) {
	Cell::operator=(other);
	if (this != &other) {
		_arrowsList = other._arrowsList;
		_isMarked = other._isMarked;
		_game = other._game;
	}
	return *this;
}

/* move assignment operator */
const AmazonotCell& AmazonotCell::operator=(AmazonotCell&& other) {
	Cell::operator=(other);
	if (this != &other) {
		_arrowsList = std::move(other._arrowsList);
		_isMarked = other._isMarked;
		_game = other._game;
		other._game = nullptr;
	}
	return *this;
}

/* the function set _arrowsList */
void AmazonotCell::setArrowsList() {
	try {
		if (_stone || _isMarked) {
			_arrowsList.clear();
		}
		else {
			_arrowsList.clear();
			AmazonotGameStone resStone(_game, 1, _pos);
			resStone.setValidMovesList();
			_arrowsList = std::move(resStone.getValidMovesList());
		}
	}
	catch (bad_alloc& ex) {
		throw;
	}
}

/* the function return true if the dst position on _arrowsList and false else */
bool AmazonotCell::isOnValidMarksCells(GameStone::Position& src, const GameStone::Position& dst) {
	for (auto itr = _arrowsList.begin(); itr != _arrowsList.end(); ++itr) {
		if (*(itr) == dst) {
			return true;
		}
	}
	return false;
}

/* the function reset the cell properties */
void AmazonotCell::resetCell() {
	_stone = nullptr;
	_isMarked = false;
}

/* the function return list for case 6 on the menu */
list<GameStone::Position>* AmazonotCell::getCase6list() {
	if (_stone)
		return nullptr;
	else
		return &(_arrowsList);
}