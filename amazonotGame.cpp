#include "amazonotGame.h"
#include "amazonotGameStone.h"
#include <iostream>
#include "generalExceptions.h"
#include "generalExceptions.h"
using namespace std;
#define rcastcc reinterpret_cast<const char*>

/* AmazonotGame constructor */
AmazonotGame::AmazonotGame(istream& in) :Game(in) {
	_requirePositions = 3;
	int countStones;
	in.read(rcastc(&countStones), sizeof(countStones));
	if (!in.good())
		throw FileException("Fail in read from file");
	vector<AmazonotGameStone*> vectorOfStones;
	vectorOfStones.reserve(countStones);
	try {
		for (int i = 0; i < countStones; i++) {
			vectorOfStones.push_back(new AmazonotGameStone(in));
			vectorOfStones[i]->setGame(this);
		}
		vector<GameStone::Position> vectorOfPositions;

		in.read(rcastc(&_countMarkCells), sizeof(_countMarkCells));
		if (!in.good())
			throw FileException("Fail in read from file");
		vectorOfStones.reserve(_countMarkCells);
		for (int i = 0; i < _countMarkCells; i++)
		{
			GameStone::Position position;
			in.read(rcastc(&(position.x)), sizeof(position.x));
			if (!in.good())
				throw FileException("Fail in read from file");
			in.read(rcastc(&(position.y)), sizeof(position.y));
			if (!in.good())
				throw FileException("Fail in read from file");
			vectorOfPositions.push_back(position);
		}

		setBoardFromFile(vectorOfStones, vectorOfPositions);
		_player1.setStoneList();
		_player2.setStoneList();
		_countMarkCells = 0;
	}
	catch (...) {
		deletVectorData(vectorOfStones);
		throw;
	}
}

/* the function delete all vector data */
void AmazonotGame::deletVectorData(vector<AmazonotGameStone*>& v) {
	int len = v.size();
	for (int i = 0; i < len; i++)
		delete v[i];
}

/* the function set the board according to the file */
void AmazonotGame::setBoardFromFile(vector<AmazonotGameStone*>& vectorOfStones, vector<GameStone::Position>& vectorOfPositions) {
	int len = vectorOfStones.size();
	for (int i = 0; i < len; i++)
	{
		_board[vectorOfStones[i]->getPosition().x - 1][vectorOfStones[i]->getPosition().y - 1].setStone(vectorOfStones[i]);
	}
	len = vectorOfPositions.size();
	for (int i = 0; i < len; i++)
	{
		Cell* cell = &(getCellOnBoard(vectorOfStones[i]->getPosition()));
		AmazonotCell* a_cell = dynamic_cast<AmazonotCell*>(cell);
		if (a_cell)
			a_cell->setMark(true);
		else
			throw bad_cast();
	}
	try {
		for (int i = 0; i < amazonot_size; i++)
		{
			for (int j = 0; j < amazonot_size; j++)
			{
				GameStone::Position pos = { i + 1,j + 1 };
				_board[i][j].setPosition(pos);
				_board[i][j].setGame(this);
				if (_board[i][j].getStone())
					_board[i][j].getStone()->setValidMovesList();
				_board[i][j].setArrowsList();
			}
		}
	}
	catch (bad_alloc& ex) {
		throw;
	}
}

/* AmazonotGame constructor */
AmazonotGame::AmazonotGame() {
	_requirePositions = 3;
	try {
		for (int i = 0; i < amazonot_size; i++)
		{
			for (int j = 0; j < amazonot_size; j++)
			{
				GameStone::Position pos = { i + 1, j + 1 };
				_board[i][j].setPosition(pos);
				_board[i][j].setGame(this);
			}
		}

		setBoard();
		_player1.setStoneList();
		_player2.setStoneList();
	}
	catch (...) {
		deleteAllStones();
		throw;
	}
}

/* the function set the game*/
void AmazonotGame::setBoard() {
	_board[6][0].setStone(new AmazonotGameStone(this, 1, GameStone::Position{ 7,1 }));
	_board[9][3].setStone(new AmazonotGameStone(this, 1, GameStone::Position{ 10,4 }));
	_board[9][6].setStone(new AmazonotGameStone(this, 1, GameStone::Position{ 10,7 }));
	_board[6][9].setStone(new AmazonotGameStone(this, 1, GameStone::Position{ 7,10 }));
	_board[3][0].setStone(new AmazonotGameStone(this, 2, GameStone::Position{ 4,1 }));
	_board[0][3].setStone(new AmazonotGameStone(this, 2, GameStone::Position{ 1,4 }));
	_board[0][6].setStone(new AmazonotGameStone(this, 2, GameStone::Position{ 1,7 }));
	_board[3][9].setStone(new AmazonotGameStone(this, 2, GameStone::Position{ 4,10 }));

	for (int i = 0; i < amazonot_size; i++)
	{
		for (int j = 0; j < amazonot_size; j++)
		{
			if (_board[i][j].getStone()) {
				_board[i][j].getStone()->setValidMovesList();
			}
			_board[i][j].setArrowsList();
		}
	}
}

/* AmazonotGame destructor */
AmazonotGame::~AmazonotGame() {
	deleteAllStones();
}

/* assignment operator */
const AmazonotGame& AmazonotGame::operator= (const AmazonotGame& other) {
	Game::operator=(other);
	if (this != &other) {
		for (int i = 0; i < amazonot_size; i++)
		{
			for (int j = 0; j < amazonot_size; j++)
			{
				if (_board[i][j].getStone())
					delete _board[i][j].getStone();

				_board[i][j] = other._board[i][j];

				if (other._board[i][j].getStone()) {
					try {
						AmazonotGameStone* a_stone = dynamic_cast<AmazonotGameStone*>(other._board[i][j].getStone());
						_board[i][j].setStone(new AmazonotGameStone(static_cast<AmazonotGameStone>(*a_stone)));
					}
					catch (bad_cast) {
						throw bad_cast();
					}
				}
			}
		}
		_countMarkCells = other._countMarkCells;
	}
	return *this;
}

/* move assignment operator */
const AmazonotGame& AmazonotGame::operator= (AmazonotGame&& other) {
	Game::operator=(other);
	if (this != &other) {
		for (int i = 0; i < amazonot_size; i++)
		{
			for (int j = 0; j < amazonot_size; j++)
			{
				if (_board[i][j].getStone())
					delete _board[i][j].getStone();

				_board[i][j] = std::move(other._board[i][j]);
				other._board[i][j].setStone(nullptr);
			}
		}
		_countMarkCells = other._countMarkCells;
		other._countMarkCells = 0;
	}
	return *this;
}

/* the function delete all the stones on the board */
void AmazonotGame::deleteAllStones() {
	for (int i = 0; i < amazonot_size; i++)
	{
		for (int j = 0; j < amazonot_size; j++)
		{
			if (_board[i][j].getStone()) {
				delete _board[i][j].getStone();
			}
			_board[i][j].setStone(nullptr);
		}
	}
}

/* the function make user move on the game and return move status */
void AmazonotGame::makeMove(GameStone::Position& src, GameStone::Position& dstMove, GameStone::Position* dstArrow) {
	if (_isOnGoing) {
		Player* opponent;
		_curPlayerTurn == &_player1 ? opponent = &_player2 : opponent = &_player1;

		GameStone* stonePtrSrc = _curPlayerTurn->getStoneFromStonesList(src);
		Cell cellArrow = getCellOnBoard(*dstArrow);

		if (!isInBoardLines(*dstArrow)) {
			throw GeneralException("invalid arrow in method make move.");
		}
		else if (!stonePtrSrc) {
			throw GeneralException("invalid position in method make move - there is no stone in src position");
		}
		else if (!stonePtrSrc->isInMovesList(dstMove)) {
			throw GeneralException("invalid move in method make move.");
		}
		else {
			getCellOnBoard(src).setStone(nullptr);
			_board[dstMove.x - 1][dstMove.y - 1].setArrowsList();
			if (!((_board[dstMove.x - 1][dstMove.y - 1]).isOnValidMarksCells(dstMove, *dstArrow))) {
				getCellOnBoard(src).setStone(stonePtrSrc);
				_board[dstMove.x - 1][dstMove.y - 1].setArrowsList();
				throw GeneralException("invalid arrow in method make move.");
			}
			getCellOnBoard(src).setStone(stonePtrSrc);
			setStoneOnBoard(dstMove, stonePtrSrc,*dstArrow);
			_curPlayerTurn->setTurn(false);
			opponent->setTurn(true);
		}
	}
}

/* the function set stones on board */
void AmazonotGame::setStoneOnBoard(const GameStone::Position& dst, GameStone* stone, const GameStone::Position& arrow) {
	if (isInBoardLines(dst) && isInBoardLines(stone->getPosition()) && isInBoardLines(arrow)) {
		_board[stone->getPosition().x - 1][stone->getPosition().y - 1].setStone(nullptr);
		_board[stone->getPosition().x - 1][stone->getPosition().y - 1].setMark(false);
		stone->setPosition(dst);
		_board[dst.x - 1][dst.y - 1].setStone(stone);
		_board[arrow.x - 1][arrow.y - 1].setMark(true);
		++_countMarkCells;
		for (int i = 0; i < amazonot_size; i++)
		{
			for (int j = 0; j < amazonot_size; j++)
			{
				if (_board[i][j].getStone())
					_board[i][j].getStone()->setValidMovesList();
				_board[i][j].setArrowsList();
			}
		}
	}
}

/* the function return the cell on board game */
Cell& AmazonotGame::getCellOnBoard(const GameStone::Position& position) {
	if (isInBoardLines(position)) {
		Cell& cell = _board[position.x - 1][position.y - 1];
		return cell;
	}
	else {
		throw invalid_argument("Invalid position in setCellOnBoard");
	}
}

/* the function return true if position on board lines and false else */
bool AmazonotGame::isInBoardLines(const GameStone::Position& position)const{
	return position.x > 0 && position.x <= amazonot_size && position.y > 0 && position.y <= amazonot_size;
}

/* the function return true if game end and false else */
bool AmazonotGame::isGameEnd() {
	return _curPlayerTurn->isGameEnd();
}

/* the function save the game to file */
void AmazonotGame::save(ostream& outFile) const {
	Game::save(outFile);
	outFile.write(rcastcc(&amazonot_stones_number), sizeof(amazonot_stones_number));
	if (!outFile.good())
		throw FileException("Fail in write to file");
	saveStonesListOfPlayer(outFile, 1);
	saveStonesListOfPlayer(outFile, 2);
	outFile.write(rcastcc(&_countMarkCells), sizeof(_countMarkCells));
	if (!outFile.good())
		throw FileException("Fail in write to file");
	saveMarkedCells(outFile);
}

/* the function save the marked cells to file */
void AmazonotGame::saveMarkedCells(ostream& outFile) const {
	for (int i = 0; i < amazonot_size; i++)
	{
		for (int j = 0; j < amazonot_size; j++)
		{
			if (_board[i][j].getIsMarked()) {
				GameStone::Position pos = { i + 1,j + 1 };
				pos.save(outFile);
			}
		}
	}
}
