#include "cell.h"
#include "damkaGameStone.h"
#include "chessGameStone.h"
#include"amazonotGameStone.h"
#include <typeinfo>
#include <iostream>
using namespace std;

/* assignment operator */
const Cell& Cell::operator=(const Cell& other) {
	if (this != &other) {
		_stone = other._stone;
	}
	return *this;
}

/* move assignment operator */
const Cell& Cell::operator=(Cell&& other) {
	if (this != &other) {
		_stone = other._stone;
		other._stone = nullptr;
	}
	return *this;
}

/* the function return list for case 6 on the menu */
list<GameStone::Position>* Cell::getCase6list() {
	if (_stone)
		return _stone->getRiskList();
	else
		return nullptr;
}