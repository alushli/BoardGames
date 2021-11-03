#include "chessKingStone.h"

/* the function create valide moves list for the cur stone */
void ChessKingStone::setValidMovesList() {
	_validMovesList.clear();
	setValidListHelper(Side::FORWARD, Side::NONE);
	setValidListHelper(Side::BACKWARD, Side::NONE);
	setValidListHelper(Side::NONE, Side::RIGTH);
	setValidListHelper(Side::NONE, Side::LEFT);
	setValidListHelper(Side::FORWARD, Side::LEFT);
	setValidListHelper(Side::FORWARD, Side::RIGTH);
	setValidListHelper(Side::BACKWARD, Side::LEFT);
	setValidListHelper(Side::BACKWARD, Side::RIGTH);
}

/* setValidListHelper help function - the function update valids positions of king */
void ChessKingStone::setValidListHelper(Side sideRow, Side sideCol) {
	try {
		Position pos = _position;
		PromotePosIndex(sideRow, sideCol, pos);
		if (_game->isInBoardLines(pos)) {
			if (!_game->getCellOnBoard(pos).getStone() || _game->getCellOnBoard(pos).getStone()->getPlayerNum() != _playerNumber) {
				if (!_game->isDangerKing(this, _position, pos))
					addPositionToList(pos);
			}
		}
	}
	catch (bad_alloc& ex) {
		_validMovesList.clear();
		throw;
	}

}

/* the function get char stone by stone status */
char ChessKingStone::getCharByType() {
	char returnVal;
	_playerNumber == 1 ? returnVal = 'k' : returnVal = 'K';
	return returnVal;
}
