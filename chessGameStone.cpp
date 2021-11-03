#include "chessGameStone.h"
#define rcastcc reinterpret_cast<const char*>

/* ChessGameStone destructor */
ChessGameStone::~ChessGameStone() {
	_validMovesList.clear();
	_riskList.clear();
}

/* setValidMovesList help function - the function update pos indexs */
void ChessGameStone::PromotePosIndex(Side sideRow, Side sideCol, Position& pos) {
	if (sideRow == Side::FORWARD)
		++pos.x;
	else if (sideRow == Side::BACKWARD)
		--pos.x;
	if (sideCol == Side::RIGTH)
		++pos.y;
	else if (sideCol == Side::LEFT)
		--pos.y;
}

/* the function add position to valid moves list */
void ChessGameStone::addPositionToList(Position& position) {
	_validMovesList.push_back(position);
}

/* the function add stone to risk list */
void ChessGameStone::addStoneToList(ChessGameStone* stone) {
	_riskList.push_back(stone->getPosition());
}

/* the function set risk list to the stone */
void ChessGameStone::setRiskList() {
	_riskList.clear();
	try {
		setRiskListHelper(Side::FORWARD, Side::NONE);
		setRiskListHelper(Side::BACKWARD, Side::NONE);
		setRiskListHelper(Side::NONE, Side::RIGTH);
		setRiskListHelper(Side::NONE, Side::LEFT);
		setRiskListHelper(Side::FORWARD, Side::LEFT);
		setRiskListHelper(Side::FORWARD, Side::RIGTH);
		setRiskListHelper(Side::BACKWARD, Side::LEFT);
		setRiskListHelper(Side::BACKWARD, Side::RIGTH);
		chechRisksKnightOfOpponent();
	}
	catch (bad_cast & ex) {
		_riskList.clear();
	}

}

/* setRiskList help function - the function update risk of stones */
void ChessGameStone::setRiskListHelper(Side sideRow, Side sideCol) {
	Position pos = _position;
	PromotePosIndex(sideRow, sideCol, pos);
	bool flag = false;
	int steps = 0;
	try {
		while (_game->isInBoardLines(pos) && !flag) {
			++steps;
			GameStone* stone = _game->getCellOnBoard(pos).getStone();
			if (stone) {
				if (stone->getPlayerNum() != _playerNumber) {
					ChessGameStone* c_stone = dynamic_cast<ChessGameStone*>(stone);
					if (c_stone) {
						if (canBeThere(c_stone->getType(), sideRow, sideCol, steps))
							addStoneToList(c_stone);
					}
				}
				flag = true;
			}
			else {
				PromotePosIndex(sideRow, sideCol, pos);
			}
		}
	}
	catch (bad_alloc & ex) {
		_riskList.clear();
		throw;
	}

}

/* the function return true if the stone can be in the cell board according to stone type and false else */
bool ChessGameStone::canBeThere(TypeOfStone type, Side sideRow, Side sideCol, int stepCount) {
	switch (type)
	{
	case ChessGameStone::TypeOfStone::KING:
		if (stepCount == 1)
			return true;
		break;
	case ChessGameStone::TypeOfStone::ROOK:
		if (sideRow == Side::NONE || sideCol == Side::NONE)
			return true;
		break;
	case ChessGameStone::TypeOfStone::BISHOP:
		if (sideRow != Side::NONE && sideCol != Side::NONE)
			return true;
		break;
	case ChessGameStone::TypeOfStone::QUEEN:
		return true;
		break;
	case ChessGameStone::TypeOfStone::PAWN:
		if (stepCount == 1 && sideRow != Side::NONE && sideCol != Side::NONE)
			if ((_playerNumber == 1 && sideRow == Side::BACKWARD) || (_playerNumber == 2 && sideRow == Side::FORWARD))
				return true;
		break;
	default:
		break;
	}
	return false;
}

/* setRiskList help function - the function update risk of knight stones */
void ChessGameStone::chechRisksKnightOfOpponent() {
	isKnightThere(Side::FORWARD, Side::LEFT);
	isKnightThere(Side::FORWARD, Side::RIGTH);
	isKnightThere(Side::BACKWARD, Side::LEFT);
	isKnightThere(Side::BACKWARD, Side::RIGTH);
	isKnightThere(Side::RIGTH, Side::FORWARD);
	isKnightThere(Side::RIGTH, Side::BACKWARD);
	isKnightThere(Side::LEFT, Side::FORWARD);
	isKnightThere(Side::LEFT, Side::BACKWARD);
}

/* the function return true if the knight on the cell and false else */
void ChessGameStone::isKnightThere(Side side2Step, Side side1Step) {
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
		GameStone* stone = _game->getCellOnBoard(pos).getStone();
		if (stone && stone->getPlayerNum() != _playerNumber) {
			ChessGameStone* c_stone = dynamic_cast<ChessGameStone*>(stone);
			if (c_stone) {
				if (c_stone->getType() == TypeOfStone::KNIGHT)
					addStoneToList(c_stone);
			}
			else {
				throw bad_cast();
			}
		}
	}
}

/* the function return true if the position on the valid moves list or false else */
bool ChessGameStone::isInMovesList(Position& position)const {
	for (auto itr = _validMovesList.begin(); itr != _validMovesList.end(); ++itr) {
		if (*(itr) == position)
			return true;
	}
	return false;
}

/* assignment operator */
const ChessGameStone& ChessGameStone::operator=(const ChessGameStone& other){
	GameStone::operator=(other);
	if (this != &other) {
		_game = other._game;
		_validMovesList = other._validMovesList;
		_riskList = other._riskList;
		_type = other._type;
	}
	return *this;
}

/* move assignment operator */
const ChessGameStone& ChessGameStone::operator=(ChessGameStone&& other){
	GameStone::operator=(other);
	if (this != &other) {
		_game = other._game;
		_validMovesList = std::move(other._validMovesList);
		_riskList = std::move(other._riskList);
		_type = other._type;
	}
	return *this;
}

/* ChessGameStone copy constructor */
ChessGameStone::ChessGameStone(const ChessGameStone& other) :GameStone(other) {
	try {
		*this = other;
	}
	catch (bad_alloc& ex) {
		_riskList.clear();
		_validMovesList.clear();
		throw;
	}
}

/* the function save the game to file */
void ChessGameStone::save(ostream& outFile) const {
	outFile.write(rcastcc(&_type), sizeof(_type));
	if (!outFile.good())
		throw FileException("Fail write to file");
	GameStone::save(outFile);
}
