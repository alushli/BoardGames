#include <iostream>
#include"gameManeger.h"
using namespace std;

int main() {
	GameManeger maneger;
	int choise;
	maneger.printMenu();
	cin >> choise;
	while (choise != 8)
	{
		maneger.manageMenu(choise);
		maneger.printMenu();
		cin >> choise;
	}
}

