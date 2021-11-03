#include "amazonotGameStone.h"
#include "player.h"

/* AmazonotGameStone destructor */
AmazonotGameStone::~AmazonotGameStone() {
	_validMovesList.clear();
}

/* assignment operator */
const AmazonotGameStone& AmazonotGameStone::operator= (const AmazonotGameStone& other) {
	GameStone::operator=(other);
	if (this != &other) {
		_game = other._game;
		_validMovesList = other._validMovesList;
	}
	return *this;
}

/* move assignment operator */
const AmazonotGameStone& AmazonotGameStone::operator= (AmazonotGameStone&& other) {
	GameStone::operator=(other);
	if (this != &other) {
		_game = other._game;
		other._game = nullptr;
		_validMovesList = other._validMovesList;
	}
	return *this;
}

/* the function return true if the position on the valid moves list and false else */
bool AmazonotGameStone::isInMovesList(Position& position)const {
	for (auto itr = _validMovesList.begin(); itr != _validMovesList.end(); ++itr) {
		if (*itr == position)
			return true;
	}
	return false;
}

/* the function create valide moves list for the cur stone */
void AmazonotGameStone::setValidMovesList()
{
	_validMovesList.clear();
	setValidListHelper(Side::FORWARD, Side::RIGTH);
	setValidListHelper(Side::FORWARD, Side::LEFT);
	setValidListHelper(Side::BACKWARD, Side::LEFT);
	setValidListHelper(Side::BACKWARD, Side::RIGTH);
	setValidListHelper(Side::NONE, Side::RIGTH);
	setValidListHelper(Side::NONE, Side::LEFT);
	setValidListHelper(Side::FORWARD, Side::NONE);
	setValidListHelper(Side::BACKWARD, Side::NONE);
}

/* setValidMovesList help function - the function update pos indexs */
void AmazonotGameStone::PromotePosIndex(Side upOrDown, Side leftOrRight, Position& pos) {
	if (upOrDown == Side::FORWARD)
		++pos.x;
	else if (upOrDown == Side::BACKWARD)
		--pos.x;
	if (leftOrRight == Side::RIGTH)
		++pos.y;
	else if (leftOrRight == Side::LEFT)
		--pos.y;
}

/* setValidMovesList help function - the function check valid posions and add to  validMovesList positions */
void AmazonotGameStone::setValidListHelper(Side upOrDown, Side leftOrRight) {
	Position pos = _position;
	PromotePosIndex(upOrDown, leftOrRight, pos);
	bool flag = false;
	try {
		while (_game->isInBoardLines(pos) && !flag) {
			AmazonotCell* a_cell = static_cast<AmazonotCell*> (&(_game->getCellOnBoard(pos)));
			if (!a_cell->getStone() && !a_cell->getIsMarked()) {
				Position* posToAdd = new Position;
				posToAdd->x = pos.x;
				posToAdd->y = pos.y;
				_validMovesList.push_back(*posToAdd);
			}
			else {
				flag = true;
			}
			PromotePosIndex(upOrDown, leftOrRight, pos);
		}
	}
	catch (bad_alloc& ex) {
		_validMovesList.clear();
	}
 }

 /* the function get char stone by stone status */
 char AmazonotGameStone::getCharByType() {
	 if (_playerNumber == 1)
		 return 'O';
	 return 'X';
 }