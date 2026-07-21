#include<iostream>
#include "snake.h"
#include<Windows.h>
#include<conio.h>
using namespace std;



int newMoveDirection;
int oldMoveDirection;
int food_x, food_y;
int snakeMatrix[Hight][Width] = { 0 };
int oldTail_x, oldTail_y;
int oldHead_x, oldHead_y;
int maxSnake = 0;


void initSnake() {
	//左右边框
	for (int i = 0; i < Hight; i++) {
		snakeMatrix[i][0] = -1;
		snakeMatrix[i][Width - 1] = -1;
	}
	//上下边框
	for (int j = 0; j < Width; j++) {
		snakeMatrix[0][j] = -1;
		snakeMatrix[Hight - 1][j] = -1;
	}

	//蛇头位置
	snakeMatrix[Hight / 2][Width / 2] = 1;
	//蛇身
	for (int i = 1; i <= 4; i++) {
		snakeMatrix[Hight / 2][Width / 2 - i] = i + 1;
		newMoveDirection = 4; //1234分别表示上下左右
	}
	createFood();
}


void drawPicture() {
	zeroXy(0, 0);
	for (int x = 0; x < Hight; x++) {
		for (int y = 0; y < Width; y++) {
			if (snakeMatrix[x][y] == 0) {
				cout << " ";
			}
			if (snakeMatrix[x][y] == -1) {
				cout << "#";
			}
			if (snakeMatrix[x][y] == 1) {
				cout << "@";
			}
			if (snakeMatrix[x][y] > 1) {
				cout << "*";
			}
			if (snakeMatrix[x][y] == -2) {
				cout << "F";
			}
		}
		cout << endl;
	}

}






void moveSnakeByDirection() {
	maxSnake = 0;
	for (int x = 1; x < Hight - 1; x++) {
		for (int y = 1; y < Width - 1; y++) {
			if (snakeMatrix[x][y] > 0) {
				snakeMatrix[x][y]++;
				if (maxSnake < snakeMatrix[x][y]) { //找蛇尾
					maxSnake = snakeMatrix[x][y];
					oldTail_x = x;
					oldTail_y = y;
				}
				if (snakeMatrix[x][y] == 2) { //找蛇头
					oldHead_x = x;
					oldHead_y = y;
				}
			}
		}
	}
	if (isCover()) {
		snakeMatrix[oldTail_x][oldTail_y] = maxSnake;
	}
	else {
		snakeMatrix[oldTail_x][oldTail_y] = 0;
	}
}


void operateSnakeByDirection() {
	oldMoveDirection = newMoveDirection;
	HideCursor();
	bool keyPressed = false;
	char input;
	if (_kbhit()) {
		input = _getch();
		switch (input) {
		case 'w':
			newMoveDirection = 1;
			keyPressed = true;
			break;
		case 's':
			newMoveDirection = 2;
			keyPressed = true;
			break;
		case 'a':
			newMoveDirection = 3;
			keyPressed = true;
			break;
		case 'd':
			newMoveDirection = 4;
			keyPressed = true;
			break;
		}
	}
	moveSnakeByDirection();
	if (keyPressed && oldMoveDirection == newMoveDirection) {
		moveSnakeByDirection();
		moveSnakeByDirection();
		moveSnakeByDirection();
		moveSnakeByDirection();
	}
}



bool isCover() {
	int newHead_x;
	int	newHead_y;
	switch (oldMoveDirection) {
	case 1:
		newHead_x = oldHead_x - 1;
		newHead_y = oldHead_y;
		break;
	case 2:
		newHead_x = oldHead_x + 1;
		newHead_y = oldHead_y;
		break;
	case 3:
		newHead_x = oldHead_x;
		newHead_y = oldHead_y - 1;
		break;
	case 4:
		newHead_x = oldHead_x;
		newHead_y = oldHead_y + 1;
		break;
	}
	// 穿越边界：从一侧出去，从对侧进来
	if (newHead_x < 1) newHead_x = Hight - 2;
	if (newHead_x > Hight - 2) newHead_x = 1;
	if (newHead_y < 1) newHead_y = Width - 2;
	if (newHead_y > Width - 2) newHead_y = 1;
	// 只检测撞到自己，撞边界不判失败
	if (snakeMatrix[newHead_x][newHead_y] > 0) {
		if(maxSnake >= (Hight-2)*(Width-2)){
			system("cls");
			cout << "恭喜通关！" << endl;
			Sleep(2000);
			exit(0);
		}
		system("cls");
		cout << "游戏失败！" << endl;
		Sleep(2000);
		exit(0);
	}
	else {
		if (snakeMatrix[newHead_x][newHead_y] == -2) {
			createFood();
			snakeMatrix[newHead_x][newHead_y] = 1;
			return true;
		}
		snakeMatrix[newHead_x][newHead_y] = 1;
		return false;
	}
}


void createFood() {
	if (maxSnake < (Hight - 2) * (Width - 2)) {
		do {
			food_x = rand() % (Hight - 2) + 1;
			food_y = rand() % (Width - 2) + 1;
		} while (snakeMatrix[food_x][food_y] != 0);
		snakeMatrix[food_x][food_y] = -2;
	}
	
}

void zeroXy(int x, int y) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(handle, pos);
}


void HideCursor() {
	CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}