#include "chessBishopStone.h"

/* the function create valide moves list for the cur stone */
void ChessBishopStone::setValidMovesList() {
	_validMovesList.clear();
	setValidListHelper(Side::FORWARD, Side::LEFT);
	setValidListHelper(Side::FORWARD, Side::RIGTH);
	setValidListHelper(Side::BACKWARD, Side::LEFT);
	setValidListHelper(Side::BACKWARD, Side::RIGTH);
}

/* setValidListHelper help function - the function update valids positions of bishop */
void ChessBishopStone::setValidListHelper(Side sideRow, Side sideCol) {
	try {
		Position pos = _position;
		PromotePosIndex(sideRow, sideCol, pos);
		bool flag = false;

		while (_game->isInBoardLines(pos) && !flag) {
			if (!_game->getCellOnBoard(pos).getStone()) {
				if (!_game->isDangerKing(this, _position, pos))
					addPositionToList(pos);
			}
			else if (_game->getCellOnBoard(pos).getStone()->getPlayerNum() != _playerNumber) {
				if (!_game->isDangerKing(this, _position, pos))
					addPositionToList(pos);
				flag = true;
			}
			else
				flag = true;
			PromotePosIndex(sideRow, sideCol, pos);
		}
	}
	catch (bad_alloc & ex) {
		_validMovesList.clear();
		throw;
	}
}

/* the function get char stone by stone status */
char ChessBishopStone::getCharByType() {
	char returnVal;
	_playerNumber == 1 ? returnVal = 'b' : returnVal = 'B';
	return returnVal;
}
