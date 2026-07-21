#include<iostream>
#include "fighter.h"
using namespace std;


int main() {


	State s = init();
	cout << "第1关" << endl;
	system("pause");
	while (s.level > 0 && s.hp>0) {
		zeroXy(0, 0);
		s = drawPicture(s);
		s = pcOperate(s);
		s =operateFighter(s);
		s = isWin(s);
	}

	cout << "game over!" << endl;
	system("pause");
}

