#include "chessPawnStone.h"

/* the function create valide moves list for the cur stone */
void ChessPawnStone::setValidMovesList() {
	int playerNum = _game->getCellOnBoard(_position).getStone()->getPlayerNum();
	_validMovesList.clear();
	if (playerNum == 1) {
		if (_position.x == rowPlayer1) {
			setValidListHelper(Side::BACKWARD, Side::NONE, 2);
		}
		else {
			setValidListHelper(Side::BACKWARD, Side::NONE, 1);
		}
		setValidListHelper(Side::BACKWARD, Side::LEFT, 1);
		setValidListHelper(Side::BACKWARD, Side::RIGTH, 1);
	}
	else if(playerNum == 2){
		if (playerNum == 2 && _position.x == rowPlayer2) {
			setValidListHelper(Side::FORWARD, Side::NONE, 2);
		}
		else {
			setValidListHelper(Side::FORWARD, Side::NONE, 1);
		}
		setValidListHelper(Side::FORWARD, Side::LEFT, 1);
		setValidListHelper(Side::FORWARD, Side::RIGTH, 1);
	}
}

/* setValidListHelper help function - the function update valids positions of pawn */
void ChessPawnStone::setValidListHelper(Side sideRow, Side sideCol, int stepNum) {
	try {
		Position pos = _position;
		if (sideCol == Side::NONE) {
			for (int i = 0; i < stepNum; i++) {
				PromotePosIndex(sideRow, sideCol, pos);
				if (_game->isInBoardLines(pos) && !_game->getCellOnBoard(pos).getStone()) {
					if (!_game->isDangerKing(this, _position, pos))
						addPositionToList(pos);
				}
			}
		}
		else {
			PromotePosIndex(sideRow, sideCol, pos);
			if (_game->isInBoardLines(pos) && _game->getCellOnBoard(pos).getStone() && _game->getCellOnBoard(pos).getStone()->getPlayerNum() != _playerNumber) {
				if (!_game->isDangerKing(this, _position, pos))
					addPositionToList(pos);
			}
		}
	}
	catch (bad_alloc) {
		_validMovesList.clear();
		throw;
	}

}

/* the function get char stone by stone status */
char ChessPawnStone::getCharByType() {
	char returnVal;
	_playerNumber == 1 ? returnVal = 'p' : returnVal = 'P';
	return returnVal;
}

