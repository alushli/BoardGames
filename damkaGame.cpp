#include "damkaGame.h"
#include "damkaGameStone.h"
#include "generalExceptions.h"

#define rcastc reinterpret_cast<char*>
#define rcastcc reinterpret_cast<const char*>

/* DamkaGame constructor */
DamkaGame::DamkaGame(istream& in):Game(in) {
	_requirePositions = 2;
	in.read(rcastc(&_countEatenStones), sizeof(_countEatenStones));
	if (!in.good())
		throw FileException("Fail read from file");
	DamkaGameStone::TypeOfStone type;
	vector<DamkaGameStone*> vectorOfStones;
	try {
		vectorOfStones.reserve(damka_stones_number - _countEatenStones);
		for (int i = 0; i < damka_stones_number - _countEatenStones; i++)
		{
			in.read(rcastc(&type), sizeof(type));
			vectorOfStones.push_back(new DamkaGameStone(in));
			if (type == DamkaGameStone::TypeOfStone::KING)
				vectorOfStones[i]->setStatus(DamkaGameStone::TypeOfStone::KING);
			vectorOfStones[i]->setGame(this);
		}
		setBoardFromFile(vectorOfStones);
		_player1.setStoneList();
		_player2.setStoneList();
	}
	catch (...) {
		deletVectorData(vectorOfStones);
		throw;
	}

}

/* the function delete vector data */
void DamkaGame::deletVectorData(vector<DamkaGameStone*>& v) {
	int len = v.size();
	for (int i = 0; i < len; i++)
		delete v[i];
}

/* the function set the board according to file */
void DamkaGame::setBoardFromFile(vector<DamkaGameStone*> &v) {
	int len = v.size();
	for (int i = 0; i < len; i++)
	{
		Cell& cell = getCellOnBoard(v[i]->getPosition());
		cell.setStone(v[i]);
	}
	setRiskAndValidAllStones();
}

/* the function set risk list and valid list to all stones */
void DamkaGame::setRiskAndValidAllStones() {
	for (int i = 0; i < damka_size; i++)
	{
		for (int j = 0; j < damka_size; j++)
		{
			if (_board[i][j].getStone()) {
				DamkaGameStone* d_stone = dynamic_cast<DamkaGameStone*>(_board[i][j].getStone());
				if (d_stone) {
					d_stone->setValidMovesList();
					d_stone->setRiskList();
				}
				else
					throw bad_cast();
			}
		}
	}
}

/* DamkaGame constructor */
DamkaGame::DamkaGame(){
	_requirePositions = 2;
	try {
		for (int i = 1; i <= damka_size; i++)
		{
			for (int j = 1; j <= damka_size; j++)
			{
				if (i <= 3) {
					if (i % 2 != j % 2) {
						GameStone::Position pos;
						pos.x = i;
						pos.y = j;
						DamkaGameStone* dStone = new DamkaGameStone(this, 1, pos);
						_board[i - 1][j - 1].setStone(dStone);
					}
				}
				else if (i >= 6) {
					if (i % 2 != j % 2) {
						GameStone::Position pos;
						pos.x = i;
						pos.y = j;
						DamkaGameStone* dStone = new DamkaGameStone(this, 2, pos);
						_board[i - 1][j - 1].setStone(dStone);
					}
				}
				else
					_board[i - 1][j - 1].setStone(nullptr);
			}
		}
		_player1.setStoneList();
		_player2.setStoneList();
		_countEatenStones = 0;
	}
	catch (...) {
		deleteAllStones();
		throw;
	}
}

/* DamkaGame destructor */
DamkaGame::~DamkaGame() {
	deleteAllStones();
}

/* the function delete all sotones on the board */
void DamkaGame::deleteAllStones() {
	for (int i = 0; i < damka_size; i++)
	{
		for (int j = 0; j < damka_size; j++)
		{
			if (_board[i][j].getStone())
				delete _board[i][j].getStone();
			_board[i][j].setStone(nullptr);
		}
	}
}

/* DamkaGame copy constructor  */
DamkaGame::DamkaGame(const DamkaGame& other):Game(other) {
	try {
		*this = other;
	}
	catch (...) {
		deleteAllStones();
		throw;
	}
}

/* assignment operator */
const DamkaGame& DamkaGame::operator= (const DamkaGame& other) {
	Game::operator=(other);
	if (this != &other)
	{
		for (int i = 0; i < damka_size; i++)
		{
			for (int j = 0; j < damka_size; j++)
			{
				if (_board[i][j].getStone())
					delete _board[i][j].getStone();

				_board[i][j] = other._board[i][j];

				if (other._board[i][j].getStone()) {
					DamkaGameStone* d_stone = dynamic_cast<DamkaGameStone*>(other._board[i][j].getStone());
					_board[i][j].setStone(new DamkaGameStone(static_cast<DamkaGameStone>(*d_stone)));
				}
			}
		}
		_countEatenStones = other._countEatenStones;
	}
	return *this;
}

/* move assignment operator */
const DamkaGame& DamkaGame::operator= (DamkaGame&& other) {
	Game::operator=(other);
	if (this != &other)
	{
		for (int i = 0; i < damka_size; i++)
		{
			for (int j = 0; j < damka_size; j++)
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

/* the function make user move on the game and return move status */
void DamkaGame::makeMove(GameStone::Position& src, GameStone::Position& dstMove, GameStone::Position* dstArrow) {
	if (_isOnGoing) {
		Player* opponent;
		_curPlayerTurn == &_player1 ? opponent = &_player2 : opponent = &_player1;

		GameStone* stonePtrSrc = _curPlayerTurn->getStoneFromStonesList(src);
		GameStone* eatenStone;
		if (!stonePtrSrc) {
			throw GeneralException("invalid position in method make move - there is no stone in src position");
		}
		else if (!stonePtrSrc->isInMovesList(dstMove)) {
			throw GeneralException("invalid move in method make move.");
		}

		eatenStone = getEatenStone(*stonePtrSrc, dstMove);
		if (eatenStone)
		{
			getCellOnBoard(eatenStone->getPosition()).setStone(nullptr);
			opponent->deletCellFromList(eatenStone);
			++_countEatenStones;
		}
		setCellOnBoard(dstMove, stonePtrSrc);

		_curPlayerTurn->setTurn(false);
		opponent->setTurn(true);
	}


}

/* the function return the eaten stone of the move */
GameStone* DamkaGame::getEatenStone(const GameStone& stone, const GameStone::Position& dst) {
	GameStone::Position resPosition = getMinusPos(stone.getPosition(), dst);
	if (resPosition.x == 1 && resPosition.y == 1) {
		return nullptr;
	}
	GameStone::Position resPosition2 = getPrevPos(stone.getPosition(), dst);
	GameStone* eateanStone = getCellOnBoard(resPosition2).getStone();
	return eateanStone;
}

/* the function calculates the difference in places between the stones and return the position of the difference */
GameStone::Position DamkaGame::getMinusPos(const GameStone::Position& src, const GameStone::Position& dst)const {
	GameStone::Position pos;
	pos.x = src.x - dst.x;
	pos.y = src.y - dst.y;
	if (pos.x < 0)
		pos.x *= -1;
	if (pos.y < 0)
		pos.y *= -1;
	return pos;
}

/* the function return the postion that between src and dst */
/* if src of the stone is KING, return the position that before the dst on hypotenuse */
GameStone::Position DamkaGame::getPrevPos(const GameStone::Position& src, const GameStone::Position& dst)const {
	GameStone::Position pos;
	if (src.x - dst.x < 0 && src.y - dst.y < 0) {
		pos.x = dst.x - 1;
		pos.y = dst.y - 1;
	}
	else if (src.x - dst.x > 0 && src.y - dst.y > 0) {
		pos.x = dst.x + 1;
		pos.y = dst.y + 1;
	}
	else if (src.x - dst.x > 0 && src.y - dst.y < 0) {
		pos.x = dst.x + 1;
		pos.y = dst.y - 1;
	}
	else {
		pos.x = dst.x - 1;
		pos.y = dst.y + 1;
	}
	return pos;
}

/* the function set dstMove cell on board */
void DamkaGame::setCellOnBoard(GameStone::Position& dst, GameStone* stone) {
	if ( isInBoardLines(dst) && isInBoardLines(stone->getPosition())){
		_board[stone->getPosition().x - 1][stone->getPosition().y - 1].setStone(nullptr);
		stone->setPosition(dst);
		_board[dst.x - 1][dst.y - 1].setStone(stone);
		if ((stone->getPlayerNum() == 1 && dst.x == damka_size) || (stone->getPlayerNum() == 2 && dst.x == 1))
		{
			DamkaGameStone* d_stone = dynamic_cast<DamkaGameStone*>(stone);
			if (d_stone)
				d_stone->setStatus(DamkaGameStone::TypeOfStone::KING);
			else
				throw bad_cast();
		}
			
		for (int i = 0; i < damka_size; i++)
		{
			for (int j = 0; j < damka_size; j++)
			{
				if (_board[i][j].getStone()) {
					DamkaGameStone* dStone = dynamic_cast<DamkaGameStone*> (_board[i][j].getStone());
					if (dStone) {
						dStone->setRiskList();
						dStone->setValidMovesList();
					}else
						throw bad_cast();
				}
			}
		}
	}
}

/* the function return cell board according to the position */
 Cell& DamkaGame::getCellOnBoard(const GameStone::Position& position) {
	 if (isInBoardLines(position))
		 return _board[position.x - 1][position.y - 1];
	 else
		 throw invalid_argument("Position out of board lines");
}

 /* the function return true if game end and false else */
 bool DamkaGame::isGameEnd() {
	return _curPlayerTurn->isGameEnd();
 }

 /* the function save the game to file */
 void DamkaGame::save(ostream& outFile) const {
	 Game::save(outFile);
	 outFile.write(rcastcc(&_countEatenStones), sizeof(_countEatenStones));
	 if (!outFile.good())
		 throw FileException("Faild write to file.");
	 saveStonesListOfPlayer(outFile, 1);
	 saveStonesListOfPlayer(outFile, 2);
 }
