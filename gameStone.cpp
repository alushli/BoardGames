#include "gameStone.h"
#define rcastcc reinterpret_cast<const char*>

/* assignment operator */
const GameStone& GameStone::operator= (const GameStone& other) {
	if (this != &other) {
		_playerNumber = other._playerNumber;
		_position = other._position;
	}
	return *this;
}

/* Game Stone constructor */
GameStone::GameStone(istream& in) {
	in.read(rcastc(&_position.x), sizeof(_position.x));
	if (!in.good())
		throw FileException("Fail read from file");
	in.read(rcastc(&_position.y), sizeof(_position.y));
	if (!in.good())
		throw FileException("Fail read from file");
	in.read(rcastc(&_playerNumber), sizeof(_playerNumber));
	if (!in.good())
		throw FileException("Fail read from file");
}

/* the function save the position to file */
void GameStone::Position::save(ostream& outFile) const {
	outFile.write(rcastcc(&x), sizeof(x));
	if (!outFile.good())
		throw FileException("Fail write to file");
	outFile.write(rcastcc(&y), sizeof(y));
	if (!outFile.good())
		throw FileException("Fail write to file");
}

/* the function save the game to file */
void GameStone::save(ostream & outFile) const {
	_position.save(outFile);
	outFile.write(rcastcc(&_playerNumber), sizeof(_playerNumber));
	if (!outFile.good())
		throw FileException("Fail write to file");
}