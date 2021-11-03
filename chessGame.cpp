#include "chessGame.h"
#include "chessBishopStone.h"
#include "chessKingStone.h"
#include "chessKnightStone.h"
#include "chessPawnStone.h"
#include "chessQueenStone.h"
#include "chessRookStone.h"
#include "vector"
#include <typeinfo>
#include "generalExceptions.h"

#define rcastc reinterpret_cast<char*>
#define rcastcc reinterpret_cast<const char*>

/* ChessGame constructor */
ChessGame::ChessGame(istream& in):Game(in) {
	_requirePositions = 2;
	in.read(rcastc(&_countEatenStones), sizeof(_countEatenStones));
	if (!in.good())
		throw FileException("Fail in read from file");
	ChessGameStone::TypeOfStone type;
	vector<ChessGameStone*> vectorOfStones;
	try {
		vectorOfStones.reserve(chess_stones_number - _countEatenStones);
		for (int i = 0; i < chess_stones_number - _countEatenStones; i++)
		{
			in.read(rcastc(&type), sizeof(type));
			if (!in.good())
				throw FileException("Fail in read from file");
			switch (type)
			{
			case ChessGameStone::TypeOfStone::KING:
				vectorOfStones.push_back(new ChessKingStone(in));
				break;
			case ChessGameStone::TypeOfStone::ROOK:
				vectorOfStones.push_back(new ChessRookStone(in));
				break;
			case ChessGameStone::TypeOfStone::BISHOP:
				vectorOfStones.push_back(new ChessBishopStone(in));
				break;
			case ChessGameStone::TypeOfStone::QUEEN:
				vectorOfStones.push_back(new ChessQueenStone(in));
				break;
			case ChessGameStone::TypeOfStone::KNIGHT:
				vectorOfStones.push_back(new ChessKnightStone(in));
				break;
			case ChessGameStone::TypeOfStone::PAWN:
				vectorOfStones.push_back(new ChessPawnStone(in));
				break;
			default:
				break;
			}
			vectorOfStones[i]->setGame(this);
		}
		setBoardFromFile(vectorOfStones);
	}
	catch (...) {
		deletVectorData(vectorOfStones);
		throw;
	}

}

/* the function delete all vector date */
void ChessGame::deletVectorData(vector<ChessGameStone*>& v) {
	int len = v.size();
	for (int i = 0; i < len; i++)
		delete v[i];
}

/* the function delete all the stones on board */
void ChessGame::deleteAllStones() {
	for (int i = 0; i < chess_size; i++)
	{
		for (int j = 0; j < chess_size; j++)
		{
			if (_board[i][j].getStone())
				delete _board[i][j].getStone();
			_board[i][j].setStone(nullptr);
		}
	}
}

/* the function set board game from file */
void ChessGame::setBoardFromFile(vector<ChessGameStone*>& v) {
	int len = v.size();
	for (int i = 0; i < len; i++)
	{
		Cell& cell = getCellOnBoard(v[i]->getPosition());
		cell.setStone(v[i]);
		if (v[i]->getType() == ChessGameStone::TypeOfStone::KING) {
			try {
				ChessKingStone* king = dynamic_cast<ChessKingStone*>(v[i]);
				v[i]->getPlayerNum() == 1 ? _king1 = king : _king2 = king;
			}
			catch (bad_cast) {
				throw FileException("fail in setBoardFromFile");
			}
		}
	}
	try {
		setValidMovesToStones();
		setRisksToStones();
		_player1.setStoneList();
		_player2.setStoneList();
	}
	catch (bad_alloc& ex) {
		throw;
	}
}

/* ChessGame destructor */
ChessGame::~ChessGame() {
	deleteAllStones();
}

/* the function make user move on the game and return move status */
void ChessGame::makeMove(GameStone::Position& src, GameStone::Position& dstMove, GameStone::Position* dstArrow) {
	if (_isOnGoing) {
		Player* opponent;
		_curPlayerTurn == &_player1 ? opponent = &_player2 : opponent = &_player1;

		GameStone* stonePtrSrc = _curPlayerTurn->getStoneFromStonesList(src);

		GameStone* eatenStone = nullptr;
		if (isInBoardLines(dstMove))
			eatenStone = getCellOnBoard(dstMove).getStone();

		if (!stonePtrSrc) {
			throw GeneralException("invalid position in method make move - there is no stone in src position");
		}
		else if (!stonePtrSrc->isInMovesList(dstMove)) {
			throw GeneralException("invalid move in method make move.");
		}
		else if (eatenStone)
		{
			opponent->deletCellFromList(eatenStone);
			_board[dstMove.x - 1][dstMove.y - 1].setStone(nullptr);
			++_countEatenStones;
		}
		if ((dstMove.x == 1 || dstMove.x == 8) && (typeid(*stonePtrSrc) == typeid(ChessPawnStone))) {
			int playerNum = stonePtrSrc->getPlayerNum();
			GameStone::Position pos = stonePtrSrc->getPosition();
			_curPlayerTurn->deletCellFromList(stonePtrSrc);
			try{
				ChessQueenStone* q_stone = new ChessQueenStone(pos, playerNum, ChessGameStone::TypeOfStone::QUEEN, this);
				stonePtrSrc = q_stone;
				_curPlayerTurn->addToStoneList(stonePtrSrc);
			}
			catch (bad_alloc & ex){
				throw;
			}
		}
		setCellOnBoard(dstMove, stonePtrSrc);
		_curPlayerTurn->setTurn(false);
		opponent->setTurn(true);
	}
}

/* the function set dstMove cell on board */
void ChessGame::setCellOnBoard(GameStone::Position& dstMove, GameStone* stone) {
	if (isInBoardLines(dstMove)) {
		getCellOnBoard(stone->getPosition()).setStone(nullptr);
		getCellOnBoard(dstMove).setStone(stone);
		stone->setPosition(dstMove);
		for (int i = 0; i < chess_size; i++)
		{
			for (int j = 0; j < chess_size; j++)
			{
				if (_board[i][j].getStone()) {
					GameStone* st = _board[i][j].getStone();
					st->setValidMovesList();
					ChessGameStone* cStone = dynamic_cast<ChessGameStone*> (_board[i][j].getStone());
					if (cStone)
						cStone->setRiskList();
					else
						throw bad_cast();
				}
			}
		}
	}
	else {
		throw invalid_argument("Invalid position in setCellOnBoard");
	}
}

/* the function set the board */
void ChessGame::setBoard() {
	setFirstRow(_player1.getPlayerNumber());
	setSecondRow(_player1.getPlayerNumber());
	setFirstRow(_player2.getPlayerNumber());
	setSecondRow(_player2.getPlayerNumber());
	setValidMovesToStones();
}

/* the function set the first row of the game for all players */
void ChessGame::setFirstRow(int playerNum) {
	int rowNumber;
	if (playerNum != 1 && playerNum != 2)
		throw invalid_argument("Invalid playerNum");
	playerNum == 1 ? rowNumber = 8 : rowNumber = 1;
	GameStone::Position pos = { rowNumber, 1 };
	_board[rowNumber - 1][0].setStone(new ChessRookStone(pos, playerNum, ChessGameStone::TypeOfStone::ROOK, this));
	pos = { rowNumber, 2 };
	_board[rowNumber - 1][1].setStone(new ChessKnightStone(pos, playerNum, ChessGameStone::TypeOfStone::KNIGHT, this));
	pos = { rowNumber, 3 };
	_board[rowNumber - 1][2].setStone(new ChessBishopStone(pos, playerNum, ChessGameStone::TypeOfStone::BISHOP, this));
	pos = { rowNumber, 4 };
	_board[rowNumber - 1][3].setStone(new ChessQueenStone(pos, playerNum, ChessGameStone::TypeOfStone::QUEEN, this));
	pos = { rowNumber, 5 };
	_board[rowNumber - 1][4].setStone(new ChessKingStone(pos, playerNum, ChessGameStone::TypeOfStone::KING, this));
	pos = { rowNumber, 6 };
	playerNum == 1 ? _king1 = static_cast<ChessKingStone*>(_board[rowNumber - 1][4].getStone()) : _king2 = static_cast<ChessKingStone*>(_board[rowNumber - 1][4].getStone());
	_board[rowNumber - 1][5].setStone(new ChessBishopStone(pos, playerNum, ChessGameStone::TypeOfStone::BISHOP, this));
	pos = { rowNumber, 7 };
	_board[rowNumber - 1][6].setStone(new ChessKnightStone(pos, playerNum, ChessGameStone::TypeOfStone::KNIGHT, this));
	pos = { rowNumber, 8 };
	_board[rowNumber - 1][7].setStone(new ChessRookStone(pos, playerNum, ChessGameStone::TypeOfStone::ROOK, this));

}

/* the function set the second row of the game for all players */
void ChessGame::setSecondRow(int playerNum) {
	int rowNumber;
	if (playerNum != 1 && playerNum != 2)
		throw invalid_argument("Invalid playerNum");
	playerNum == 1 ? rowNumber = 7 : rowNumber = 2;
	GameStone::Position pos;
	for (int i = 1; i <= chess_size; i++) {
		pos = { rowNumber, i };
		_board[rowNumber - 1][i - 1].setStone(new ChessPawnStone(pos, playerNum, ChessGameStone::TypeOfStone::PAWN, this));
	}
}

/* the function set valid move list for all stones */
void ChessGame::setValidMovesToStones() {
	for (int i = 0; i < chess_size; i++) {
		for (int j = 0; j < chess_size; j++) {
			if (_board[i][j].getStone()) {
				_board[i][j].getStone()->setValidMovesList();
			}
		}
	}
}

/* the function set risk list for all stones */
void ChessGame::setRisksToStones() {
	for (int i = 0; i < chess_size; i++) {
		for (int j = 0; j < chess_size; j++) {
			if (_board[i][j].getStone()) {
				ChessGameStone* c_stone = dynamic_cast<ChessGameStone*>(_board[i][j].getStone());
				if (c_stone)
					c_stone->setRiskList();
				else
					throw bad_cast();
			}
		}
	}
}

/* ChessGame constructor */
ChessGame::ChessGame(){
	_requirePositions = 2;
	try {
		setBoard();
		_player1.setStoneList();
		_player2.setStoneList();
		_countEatenStones = 0;
	}
	catch (...) {
		deleteAllStones();
		throw;
	}
}

/* ChessGame copy constructor */
ChessGame::ChessGame(ChessGame& other) :Game(other) {
	try {
		*this = other;
	}
	catch (...) {
		deleteAllStones();
		throw;
	}
}

/* assignment operator */
const ChessGame& ChessGame::operator= (const ChessGame& other) {
	Game::operator=(other);
	if (this != &other)
	{
		_king1 = other._king1;
		_king2 = other._king2;
		for (int i = 0; i < chess_size; i++)
		{
			for (int j = 0; j < chess_size; j++)
			{
				if (_board[i][j].getStone())
					delete _board[i][j].getStone();

				_board[i][j] = other._board[i][j];

				if (other._board[i][j].getStone()) {
					_board[i][j].setStone(allocNewChessStoneByType(static_cast<ChessGameStone*>( other._board[i][j].getStone())));
				}
			}
		}
		_countEatenStones = other._countEatenStones;
	}
	return *this;
}

/* move assignment operator */
const ChessGame& ChessGame::operator= (ChessGame&& other) {
	Game::operator=(other);
	if (this != &other)
	{
		_king1 = other._king1;
		other._king1 = nullptr;
		_king2 = other._king2;
		other._king2 = nullptr;
		for (int i = 0; i < chess_size; i++)
		{
			for (int j = 0; j < chess_size; j++)
			{
				if (_board[i][j].getStone())
					delete _board[i][j].getStone();
				_board[i][j] = std::move(other._board[i][j]);
				other._board[i][j].setStone(nullptr);
			}
		}
		_countEatenStones = other._countEatenStones;
		other._countEatenStones = 0;
	}
	return *this;
}

/* the function return new stone by type */
ChessGameStone* ChessGame::allocNewChessStoneByType(ChessGameStone* stone) {
	if (stone) {
		ChessGameStone::TypeOfStone type = stone->getType();
		int playerNum = stone->getPlayerNum();
		GameStone::Position pos = stone->getPosition();
		switch (type)
		{
		case ChessGameStone::TypeOfStone::KING:
			return new ChessKingStone(pos, playerNum, ChessGameStone::TypeOfStone::KING, this);
			break;
		case ChessGameStone::TypeOfStone::ROOK:
			return new ChessRookStone(pos, playerNum, ChessGameStone::TypeOfStone::ROOK, this);
			break;
		case ChessGameStone::TypeOfStone::BISHOP:
			return new ChessRookStone(pos, playerNum, ChessGameStone::TypeOfStone::BISHOP, this);
			break;
		case ChessGameStone::TypeOfStone::QUEEN:
			return new ChessQueenStone(pos, playerNum, ChessGameStone::TypeOfStone::QUEEN, this);
			break;
		case ChessGameStone::TypeOfStone::KNIGHT:
			return new ChessKnightStone(pos, playerNum, ChessGameStone::TypeOfStone::KNIGHT, this);
			break;
		case ChessGameStone::TypeOfStone::PAWN:
			return new ChessPawnStone(pos, playerNum, ChessGameStone::TypeOfStone::PAWN, this);
			break;
		default:
			break;
		}
	}
	else
		return nullptr;

}

/* the function return cell board according to the position */
Cell& ChessGame::getCellOnBoard(const GameStone::Position& position) {
	if (isInBoardLines(position))
		return _board[position.x - 1][position.y - 1];
	else
		throw invalid_argument("Position out of board");
}

/* the function return true if the move danger the king and false else */
bool ChessGame::isDangerKing(ChessGameStone* stone, const GameStone::Position& src, const GameStone::Position& dst) {
	bool returnVal = false;
	GameStone::Position startPos = src;
	ChessKingStone* king;
	if (stone) {
		stone->getPlayerNum() == 1 ? king = _king1 : king = _king2;
		getCellOnBoard(stone->getPosition()).setStone(nullptr);
		stone->setPosition(dst);
		GameStone* dst_stone = getCellOnBoard(dst).getStone();
		getCellOnBoard(dst).setStone(stone);
		king->setRiskList();
		king->getRiskList()->empty() ? returnVal = false : returnVal = true;
		getCellOnBoard(startPos).setStone(stone);
		stone->setPosition(startPos);
		getCellOnBoard(dst).setStone(dst_stone);
		king->setRiskList();
	}
	return returnVal;
}

/* the function return true if game end and false else */
bool ChessGame::isGameEnd() {
	return _curPlayerTurn->isGameEnd();
}

/* the function save the game to file */
void ChessGame::save(ostream& outFile) const {
	Game::save(outFile);
	outFile.write(rcastcc(&_countEatenStones), sizeof(_countEatenStones));
	if (outFile.good()) {
		saveStonesListOfPlayer(outFile, 1);
		saveStonesListOfPlayer(outFile, 2);
	}
	else {
		throw FileException("Fail write to file");
	}
}