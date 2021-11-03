#include "gameManeger.h"
#include <typeinfo>
#include <iostream>
#include <fstream>
#include "fileExceptions.h"
using namespace std;
#define rcastc reinterpret_cast<char*>
#define rcastcc reinterpret_cast<const char*>

/* assignment operator */
const GameManeger& GameManeger::operator= (const GameManeger& other) {
	if (this != &other) {
		deleteGamesList();
		try {
			_listOfGames = other._listOfGames;
			_cur = other._cur;
		}
		catch(bad_alloc & ex){
			badAllocHandle(ex);
		}

	}
	return *this;
}

/* move assignment operator */
const GameManeger& GameManeger::operator=(GameManeger&& other) {
	if (this != &other) {
		deleteGamesList();
		_listOfGames = std::move(other._listOfGames);
		_cur = other._cur;
	}
	return *this;
}

/* the function print the menu */
void GameManeger::printMenu() {
	cout << "1.Add new game" << endl;
	cout << "2.Move to the next game" << endl;
	cout << "3.Delete current game" << endl;
	cout << "4.Show current game detailes" << endl;
	cout << "5.Show valid moves list" << endl;
	cout << "6.Show risk list" << endl;
	cout << "7.Make a move" << endl;
	cout << "8.Exit" << endl;
	cout << "9.Save game" << endl;
	cout << "10.Load game" << endl;
}

/* the function ask for game type */
void  GameManeger::chooseGameType() {
	int choise;
	cout << "Please choose game: 1 for Damka, 2 for Amazonot, 3 for Chess" << endl;
	cin >> choise;
	while (choise<1 || choise > 3)
	{
		cout << "Invalid choise" << endl;
		cout << "Please choose game: 1 for Damka, 2 for Amazonot, 3 for Chess" << endl;
		cin >> choise;
	}
	try
	{
		switch (choise)
		{
		case 1:
			_listOfGames.push_back(new DamkaGame());
			break;
		case 2:
			_listOfGames.push_back(new AmazonotGame());
			break;
		case 3:
			_listOfGames.push_back(new ChessGame());
			break;
		default:
			break;
		}
		_cur = --(_listOfGames.end());
	}
	catch (bad_alloc & ex)
	{
		badAllocHandle(ex);
	}
	catch (GeneralException& ex) {
		cout << ex.what() << ex.getMessege()<< endl;
	}
	catch (exception& ex) {
		cout << ex.what() << endl;
	}
}

/* the function delete the game */
void GameManeger::badAllocHandle(bad_alloc& ex) {
	deleteGamesList();
	cout << ex.what() << endl;
	exit(1);
}

/* the function update _cur to the next game */
void GameManeger::nextGame() {
	if (_cur == --_listOfGames.end())
		_cur = _listOfGames.begin();
	else
		++_cur;
}

/* the function delete the cur game */
void GameManeger::deleteCurGame() {
	if (_cur == --_listOfGames.end()) {
		_listOfGames.pop_back();
		_cur = _listOfGames.begin();
	}
	else
		_cur = _listOfGames.erase(_cur);
}

/* the function manage the player choose */
void GameManeger::manageMenu(int choose) {
	const char* errorMassege = "There is no game in game list";
	switch (choose)
	{
	case 1:
		chooseGameType();
		break;
	case 2:
		if (!_listOfGames.empty())
			nextGame();
		else
			cout << errorMassege << endl;
		break;
	case 3:
		if (!_listOfGames.empty())
			deleteCurGame();
		else
			cout << errorMassege << endl;
		break;
	case 4:
		if (!_listOfGames.empty())
			printCurGameInfo();
		else
			cout << errorMassege << endl;
		break;
		
	case 5:
		if (!_listOfGames.empty()) {
			try {case5Or6(5);}
			catch (invalid_argument& ex) {cout << ex.what() << endl;}	
		}
		else
			cout << errorMassege << endl;
		break;
		
	case 6:
		if (!_listOfGames.empty()) {
			try{case5Or6(6);}
			catch (invalid_argument& ex){cout << ex.what() << endl;}
		}
		else
			cout << errorMassege << endl;
		break;
	case 7:
		if (!_listOfGames.empty()) {
			try{case7();}
			catch (bad_alloc& ex){badAllocHandle(ex);}
		}
		else
			cout << errorMassege << endl;
		break;
	case 8:
		break;
	case 9:
		if (!_listOfGames.empty()) {
			getFileName(9);
		}
		else
			cout << errorMassege << endl;
		break;
	case 10:
			getFileName(10);
		break;
	default:
		cout << "Invalid choise" << endl;
		break;
	}
}

/* the function print the cur game information */
void GameManeger::printCurGameInfo()const {
	Game* game = *_cur;
	cout << "Game Board:" << endl;
	printCurBoard();
	cout << endl;
	cout << "Game Status:";
	if (game->getGameStatus()) {
		cout << "On goning" << endl;
		cout << "Current player turn: Player" << game->getCurPlayer()->getPlayerNumber() << endl;
	}
	else {
		cout << "End" << endl;
		cout << "The winner is: Player" << game->getWinner()->getPlayerNumber() << endl;
	}
}

/* the function print the board of the cur game */
void GameManeger::printCurBoard()const {
	cout << *_cur;
}

/* operator cout to print the board */
ostream& operator<<(ostream& os, Game* game) {
	int size = game->getBoardSize();
	os << "   ";
	for (int i = 0; i < size; i++) {
		os << i + 1 << " ";
	}

	os << "  \n";
	for (int i = 0; i <= size; i++)
		os << "--";
	os << endl;
	for (int i = 1; i <= size; i++)
	{
		for (int j = 0; j <= size; j++)
		{
			GameStone::Position pos = { i, j };
			GameStone* stone;
			if (j == 0) {
				if (i >= 10)
					os << i << "|";
				else
					os << i << " |";
			}

			else {
				cout << game->getCellOnBoard(pos);
			}
		}
		os << endl;
	}
	for (int i = 0; i <= size; i++)
		os << "--";
	return os;
}

/* operator cout to print the cell */
ostream& operator<<(ostream& os, Cell& cell) {
	GameStone* stone = cell.getStone();
	if (!stone) 
		os << " |";
	else 
		os << stone->getCharByType() << "|";
	return os;
}

/* operator cout to print the stone */
ostream& operator<<(ostream& os, GameStone& stone) {
	GameStone::Position pos = stone.getPosition();
	os << "{" << pos.x << "," << pos.y << "}";
	return os;
}

/* operator cout to print the position */
ostream& operator<<(ostream& os, GameStone::Position & position) {
	os << "{" << position.x << "," << position.y << "}";
	return os;
}

/* the function execute case 5 or 6 from the menu */
void GameManeger::case5Or6(int choose) const{
	GameStone::Position pos;
	if ((**_cur).getGameStatus() == true) {
		cout << "Please enter position (row and col)" << endl;
		cin >> pos;
		if (!(**_cur).isInBoardLines(pos)) {
			throw invalid_argument("Invalid position input in case5Or6 method");
		}
		else {
			if (choose == 5) {
				if (!(**_cur).getCellOnBoard(pos).getStone())
					throw invalid_argument("Invalid position input in case5Or6 method - There is no stone in this position");
				else {
					cout << "Valid moves:" << endl;
					printValidMovesList(pos);
				}
			}
			else if(choose == 6) {
				Cell* cell = &((**_cur).getCellOnBoard(pos));
				list<GameStone::Position>* list;
				list = cell->getCase6list();
				if (!list)
					throw invalid_argument("Invalid position input in case5Or6 method");
				else {
					cout << "Risk list positions:" << endl;
					printRiskList(list);
				}
			}
			else {
				throw invalid_argument("Invalid argument in case5Or6 method, should get 5 or 6");
			}
		}
	}
	else
		cout << "The game is already ended" << endl;
}

/* the function print valid moves list of the stone in the get position */
void GameManeger::printValidMovesList(GameStone::Position& position)const {
	GameStone* stone = (**_cur).getCellOnBoard(position).getStone();
	list<GameStone::Position> list = stone->getValidMovesList();
	if(list.empty())
		cout<< "list is empty";
	else {
		for (auto itr = list.begin(); itr != list.end(); ++itr)
			cout << *itr;
	}

	cout << endl;
}

/* the function print risk list of the stone in the get position */
void GameManeger::printRiskList(list<GameStone::Position>* list)const {
	if (list) {
		if (list->empty())
			cout << "list is empty";
		for (auto itr = list->begin(); itr != list->end(); ++itr)
			cout << *itr;
		cout << endl;
	}
	else
		throw invalid_argument("risk list is null");

}

/* operator cin to position */
istream& operator>>(istream& is, GameStone::Position& position) {
	is >> position.x >> position.y;
	return is;
}

/* the function make user move on the cur game and update the cur player */
void GameManeger::makeMove(GameStone::Position* positions) {
	try {
		if ((**_cur).getRequirePositions() == 2)
			(**_cur).makeMove(positions[0], positions[1], nullptr);
		else
			(**_cur).makeMove(positions[0], positions[1], &(positions[2]));
		(**_cur).setCurPlayer((**_cur).getCurPlayer()->getPlayerNumber() == 1 ? 2 : 1);

		if ((**_cur).isGameEnd()) {
			int winNumber;
			(**_cur).getCurPlayer()->getPlayerNumber() == 1 ? winNumber = 2 : winNumber = 1;
			(**_cur).setWinner(winNumber);
			(**_cur).setGameStatus(false);
			(**_cur).getCurPlayer()->setStatus(Player::PlayerStatus::LOSS);
			(**_cur).setCurPlayer((**_cur).getCurPlayer()->getPlayerNumber() == 1 ? 2 : 1);
			(**_cur).getCurPlayer()->setStatus(Player::PlayerStatus::WIN);
		}
	}
	catch (GeneralException& ex) {
		cout << ex.what() << ex.getMessege() << endl;
	}
	catch (bad_alloc& ex) {
		badAllocHandle(ex);
	}
	catch (exception& ex) {
		cout << ex.what() << endl;
	}
}

/* the function execute case 7 from the menu */
void GameManeger::case7() {
	int numOfPositionsForMove = (**_cur).getRequirePositions();
	try{
		GameStone::Position* positions = new GameStone::Position[numOfPositionsForMove];
		if ((**_cur).getGameStatus()) {
			for (int i = 0; i < numOfPositionsForMove; i++)
			{
				cout << "Enter position (row, col):" << endl;
				cin >> positions[i];
			}
		}
		makeMove(positions);
		delete[] positions;
	}
	catch (bad_alloc& ex) {
		throw;
	}

}

/* the function load new game from file */
void GameManeger::loadNewGame(string & fName) {
	ifstream infile(fName, ios::binary);
	if (!infile)
		throw FileException("File not open");
	GamesType type;
	Game* node = nullptr;
	infile.read(rcastc(&type), sizeof(type));
	if (infile.good()) {
		try {
			switch (type)
			{
			case GameManeger::GamesType::DAMKA:
				node = new DamkaGame(infile);
				break;
			case GameManeger::GamesType::CHESS:
				node = new ChessGame(infile);
				break;
			case GameManeger::GamesType::AMAZONOT:
				node = new AmazonotGame(infile);
				break;
			default:
				break;
			}
			_listOfGames.push_back(node);

			_cur = --_listOfGames.end();
		}
		catch(bad_alloc & ex){
			throw;
		}
		catch (FileException& ex) {
			infile.close();
			throw;
		}
		infile.close();
	}
	else {
		infile.close();
		throw FileException("Error in read in loadNewGame method");
	}
}

/* the function save the game to file */
void GameManeger::saveGame(string& fName) {
	GamesType type;
	if (typeid(**_cur) == typeid(DamkaGame))
		type = GamesType::DAMKA;
	else if (typeid(**_cur) == typeid(AmazonotGame))
		type = GamesType::AMAZONOT;
	else if (typeid(**_cur) == typeid(ChessGame))
		type = GamesType::CHESS;
	else
		throw bad_typeid();
	ofstream outFile(fName, ios::binary);
	if (!outFile)
		throw FileException("File not open");
	outFile.write(rcastc(&type), sizeof(type));
	if (!outFile.good()) {
		outFile.close();
		throw FileException("Fail write to file - method saveGame");
	}
	try {
		(*_cur)->save(outFile);
	}
	catch (FileException& ex) {
		outFile.close();
		throw;
	}
	outFile.close();
}

/* the function request file name */
void GameManeger::getFileName(int case9or10) {
	string fName;
	cout << "Please enter file name" << endl;
	cin >> fName;
	try{
		case9or10 == 9 ? saveGame(fName) : loadNewGame(fName);
	}
	catch (bad_alloc & ex){
		badAllocHandle(ex);
	}
	catch (FileException& ex) {
		cout << ex.what() << ex.getMessege() << endl;
	}
	catch (bad_typeid& ex) {
		cout << ex.what() <<  endl;
	}

}