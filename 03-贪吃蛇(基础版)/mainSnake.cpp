#include<iostream>
#include "snake.h"
#include<Windows.h>
using namespace std;

int main() {
	initSnake();
	while (1) {
		drawPicture();
		//		pcOperate();
		Sleep(400);
		operateSnakeByDirection();
	}
	system("pause");
}
