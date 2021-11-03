#include "chessKnightStone.h"

/* the function create valide moves list for the cur stone */
void ChessKnightStone::setValidMovesList() {
	_validMovesList.clear();
	setValidListHelper(Side::FORWARD, Side::LEFT);
	setValidListHelper(Side::FORWARD, Side::RIGTH);
	setValidListHelper(Side::BACKWARD, Side::LEFT);
	setValidListHelper(Side::BACKWARD, Side::RIGTH);
	setValidListHelper(Side::RIGTH, Side::FORWARD);
	setValidListHelper(Side::RIGTH, Side::BACKWARD);
	setValidListHelper(Side::LEFT, Side::FORWARD);
	setValidListHelper(Side::LEFT, Side::BACKWARD);
}

/* setValidListHelper help function - the function update valids positions of knight */
void ChessKnightStone::setValidListHelper(Side side2Step, Side side1Step) {
	try {
		Position pos = _position;
		for (int i = 0; i < 2; i++) {
			if (side2Step == Side::FORWARD || side2Step == Side::BACKWARD)
				PromotePosIndex(side2Step, Side::NONE, pos);
			else
				PromotePosIndex(Side::NONE, side2Step, pos);
		}
		if (side1Step == Side::FORWARD || side1Step == Side::BACKWARD)
			PromotePosIndex(side1Step, Side::NONE, pos);
		else
			PromotePosIndex(Side::NONE, side1Step, pos);

		if (_game->isInBoardLines(pos)) {
			if (!_game->getCellOnBoard(pos).getStone() || _game->getCellOnBoard(pos).getStone()->getPlayerNum() != _playerNumber) {
				if (!_game->isDangerKing(this, _position, pos))
					addPositionToList(pos);
			}
		}
	}
	catch (bad_alloc & ex) {
		_validMovesList.clear();
		throw;
	}

}

/* the function get char stone by stone status */
char ChessKnightStone::getCharByType() {
	char returnVal;
	_playerNumber == 1 ? returnVal = 'n' : returnVal = 'N';
	return returnVal;
}
