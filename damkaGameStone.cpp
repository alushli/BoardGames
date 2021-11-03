#include "damkaGameStone.h"
#include <iostream>
#define rcastcc reinterpret_cast<const char*>

/* DamkaGameStone constructor */
DamkaGameStone::DamkaGameStone(DamkaGame* game, int playarNumber, Position _position):GameStone(playarNumber, _position), _type(TypeOfStone::REGULAR), _game(game){
	if (_playerNumber == 1) {
		if (_position.x == end_board_rows_player_1)
			setValidMovesList();
	}
	else if(_playerNumber == 2) {
		if (_position.x == start_board_rows_player_2)
			setValidMovesList();
	}
	else {
		_validMovesList.clear();
		_riskList.clear();
		throw invalid_argument("Invalid player number.");
	}
}

/* DamkaGameStone copy constructor */
DamkaGameStone::DamkaGameStone(const DamkaGameStone& other) :GameStone(other) {
	try {
		*this = other;
	}
	catch (bad_alloc& ex) {
		_validMovesList.clear();
		_riskList.clear();
		throw;
	}
}

/* DamkaGameStone destructor */
DamkaGameStone::~DamkaGameStone() {
	_validMovesList.clear();
}

/* assignment operator */
const DamkaGameStone& DamkaGameStone::operator= (const DamkaGameStone& other) {
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
const DamkaGameStone& DamkaGameStone::operator= (DamkaGameStone&& other) {
	GameStone::operator=(other);
	if (this != &other) {
		_game = other._game;
		_validMovesList = std::move(other._validMovesList);
		_riskList = std::move(other._riskList);
		_type = other._type;
	}
	return *this;
}

/* the function find the stones that mack risk on the cur stone and update risks list */
void DamkaGameStone::findRiskBySlantAndByDirection(Side direction, Side slant) {
	int resCount = 0;
	bool flag = true;
	Position resPosition, backPosition;
	setBackPositionBySlantAndDirection(backPosition, slant, direction);
	resPosition = _position;

	updatePositionInLoopHelper(resPosition, slant, direction);
	while (_game->isInBoardLines(resPosition) && flag) {
		if (_game->getCellOnBoard(resPosition).getStone())
		{
			flag = false;
			if (_game->getCellOnBoard(resPosition).getStone()->getPlayerNum() != _playerNumber)
			{
				DamkaGameStone* dStone = dynamic_cast<DamkaGameStone*>(_game->getCellOnBoard(resPosition).getStone());
				if (dStone && dStone->getType()== TypeOfStone::KING)
				{
					if (_game->isInBoardLines(backPosition) && !(_game->getCellOnBoard(backPosition).getStone())) {
						Position pos = dStone->getPosition();
						_riskList.push_back(pos);
					}
				}
				else {
					if (resCount == 0) {
						if (_game->isInBoardLines(backPosition) && !_game->getCellOnBoard(backPosition).getStone()) {
							Position pos = dStone->getPosition();
							_riskList.push_back(pos);
						}
					}
				}
			}
		}
		resCount++;
		updatePositionInLoopHelper(resPosition, slant, direction);
	}
}

/* the function update the position by slant and direction */
/* this is helper function to findRiskBySlantAndByDirection */
void DamkaGameStone::updatePositionInLoopHelper(Position& position, Side slant, Side direction)const {
	if (slant == Side::RIGTH && direction == Side::FORWARD) {
		++position.x;
		++position.y;
	}
	else if (slant == Side::RIGTH && direction == Side::BACKWARD) {
		--position.x;
		--position.y;
	}
	else if (slant == Side::LEFT && direction == Side::FORWARD)
	{
		--position.x;
		++position.y;
	}
	else
	{
		++position.x;
		--position.y;
	}
}

/* the function update the position to the position of the back position hypotenuse by slant and direction*/
void DamkaGameStone::setBackPositionBySlantAndDirection(Position& position, Side slant, Side direction)const {
	if (slant == Side::RIGTH && direction == Side::FORWARD) {
		position.x = _position.x - 1;
		position.y = _position.y - 1;
	}
	else if (slant == Side::RIGTH && direction == Side::BACKWARD) {
		position.x = _position.x + 1;
		position.y = _position.y + 1;
	}
	else if (slant == Side::LEFT && direction == Side::FORWARD) {
		position.x = _position.x + 1;
		position.y = _position.y - 1;
	}
	else
	{
		position.x = _position.x - 1;
		position.y = _position.y + 1;
	}
}

/* the function update thr risks list */
void DamkaGameStone::setRiskList()
{
	_riskList.clear();
	try {
		findRiskBySlantAndByDirection(Side::FORWARD, Side::RIGTH);
		findRiskBySlantAndByDirection(Side::FORWARD, Side::LEFT);
		findRiskBySlantAndByDirection(Side::BACKWARD, Side::RIGTH);
		findRiskBySlantAndByDirection(Side::BACKWARD, Side::LEFT);
	}
	catch (bad_cast& ex) {
		_riskList.clear();
	}
	
}

/* the function create valide moves list for the cur stone */
void DamkaGameStone::setValidMovesList() {
	_validMovesList.clear();
	try {
		if (_type == TypeOfStone::REGULAR) {
			checkOneStep(_position, Side::LEFT);
			checkOneStep(_position, Side::RIGTH);
			checkTwoStep(_position, Side::LEFT);
			checkTwoStep(_position, Side::RIGTH);
		}
		else {
			checkKingStep(_position, Side::LEFT, Side::FORWARD);
			checkKingStep(_position, Side::LEFT, Side::BACKWARD);
			checkKingStep(_position, Side::RIGTH, Side::FORWARD);
			checkKingStep(_position, Side::RIGTH, Side::BACKWARD);
		}
	}
	catch (bad_cast& ex) {
		_validMovesList.clear();
		throw;
	}
	catch (bad_alloc& ex) {
		_validMovesList.clear();
		throw;
	}
}

/* the function update the moves list when the stone can move 1 step - REGULAR with no eating */
void DamkaGameStone::checkOneStep(const Position& position,  Side side) {
	
	Position oneStepPos;
	if (_playerNumber == 1) {
		if (side == Side::LEFT) {
			oneStepPos.x = position.x + 1;
			oneStepPos.y = position.y - 1;
		}
		else if (side == Side::RIGTH) {
			oneStepPos.x = position.x + 1;
			oneStepPos.y = position.y + 1;
		}
	}
	else {
		if (side == Side::LEFT) {
			oneStepPos.x = position.x - 1;
			oneStepPos.y = position.y - 1;
		}
		else if (side == Side::RIGTH) {
			oneStepPos.x = position.x - 1;
			oneStepPos.y = position.y + 1;
		}
	}
	if (_game->isInBoardLines(oneStepPos) && _game->getCellOnBoard(oneStepPos).getStone() == nullptr)
		_validMovesList.push_back(oneStepPos);
}

/* the function update the moves list when the stone can move 2 step - REGULAR with eating */
void DamkaGameStone::checkTwoStep(const Position& posiotion, Side side) {

	Position oneStepPos, twoStepPos;
	if (_playerNumber == 1) {
		if (side == Side::LEFT) {
			oneStepPos.x = posiotion.x + 1;
			oneStepPos.y = posiotion.y - 1;
			twoStepPos.x = posiotion.x + 2;
			twoStepPos.y = posiotion.y - 2;
		}
		else if (side == Side::RIGTH) {
			oneStepPos.x = posiotion.x + 1;
			oneStepPos.y = posiotion.y + 1;
			twoStepPos.x = posiotion.x + 2;
			twoStepPos.y = posiotion.y + 2;
		}
	}
	else {
		if (side == Side::LEFT) {
			oneStepPos.x = posiotion.x - 1;
			oneStepPos.y = posiotion.y - 1;
			twoStepPos.x = posiotion.x - 2;
			twoStepPos.y = posiotion.y - 2;
		}
		else if (side == Side::RIGTH) {
			oneStepPos.x = posiotion.x - 1;
			oneStepPos.y = posiotion.y + 1;
			twoStepPos.x = posiotion.x - 2;
			twoStepPos.y = posiotion.y + 2;
		}
	}
	if (_game->isInBoardLines(twoStepPos) && _game->getCellOnBoard(twoStepPos).getStone() == nullptr) {
		if (_game->getCellOnBoard(oneStepPos).getStone() != nullptr && _game->getCellOnBoard(oneStepPos).getStone()->getPlayerNum() != _playerNumber)
			_validMovesList.push_back(twoStepPos);
	}
}

/* the function update the moves list when the stone is KING */
void DamkaGameStone::checkKingStep(const Position& posiotion, Side sideY, Side sideX) {
	Position nextPos = posiotion;
	bool canMove = true;
	setPointStepAccordingPlayer(nextPos, sideY, sideX);
	while (_game->isInBoardLines(nextPos) && canMove) {
		if (_game->getCellOnBoard(nextPos).getStone() == nullptr)
			_validMovesList.push_back(nextPos);
		else {
			if (_game->getCellOnBoard(nextPos).getStone()->getPlayerNum() != _playerNumber) {
				Position enemyPos = nextPos;
				setPointStepAccordingPlayer(nextPos, sideY, sideX);
				if (_game->isInBoardLines(nextPos) && _game->getCellOnBoard(nextPos).getStone() == nullptr) 
					_validMovesList.push_back(nextPos);
			}
			canMove = false;
		}
		setPointStepAccordingPlayer(nextPos, sideY, sideX);
	}
}

/* the function update the position according to the player and move of the stone to sideX and sideY */
void DamkaGameStone::setPointStepAccordingPlayer(Position& posiotion, Side sideY, Side sideX) {
	
	if (_playerNumber == 1) {
		if (sideY == Side::LEFT && sideX == Side::FORWARD) {
			posiotion.x = posiotion.x + 1;
			posiotion.y = posiotion.y + 1;
		}
		else if (sideY == Side::LEFT && sideX == Side::BACKWARD) {
			posiotion.x = posiotion.x - 1;
			posiotion.y = posiotion.y + 1;
		}
		else if (sideY == Side::RIGTH && sideX == Side::FORWARD) {
			posiotion.x = posiotion.x + 1;
			posiotion.y = posiotion.y - 1;
		}
		else {
			posiotion.x = posiotion.x - 1;
			posiotion.y = posiotion.y - 1;
		}
	}
	else {
		if (sideY == Side::LEFT && sideX == Side::FORWARD) {
			posiotion.x = posiotion.x - 1;
			posiotion.y = posiotion.y + 1;
		}
		else if (sideY == Side::LEFT && sideX == Side::BACKWARD) {
			posiotion.x = posiotion.x + 1;
			posiotion.y = posiotion.y + 1;
		}
		else if (sideY == Side::RIGTH && sideX == Side::FORWARD) {
			posiotion.x = posiotion.x - 1;
			posiotion.y = posiotion.y - 1;
		}
		else {
			posiotion.x = posiotion.x + 1;
			posiotion.y = posiotion.y - 1;
		}
	}
}

/* the function return true if the position on the valid moves list or false else */
bool DamkaGameStone::isInMovesList(Position & position)const {
	for (auto itr = _validMovesList.begin(); itr != _validMovesList.end(); ++itr) {
		if (*(itr) == position)
			return true;
	}
	return false;
}

/* the function get char stone by stone status */
char DamkaGameStone::getCharByType() {
	if (_type == TypeOfStone::KING) {
		if (_playerNumber == 1)
			return 'O';
		return 'X';
	}
	else {
		if (_playerNumber == 1)
			return 'o';
		return 'x';
	}
}

/* the function save the game to file */
void DamkaGameStone::save(ostream& outFile) const {
	outFile.write(rcastcc(&_type), sizeof(_type));
	if (!outFile.good())
		throw FileException("Fail write to file");
	GameStone::save(outFile);
}