#include "fighter.h"
#include<iostream>
#include<conio.h>
#include<Windows.h>
#include<cstring>
using namespace std;

int fighter_x, fighter_y;
int bullet_x, bullet_y;
int enemy_x[MAX_ENEMY], enemy_y[MAX_ENEMY];
int planeMatrix[height][width] = { 0 };
int speed = 0;
int activeEnemyCount = 0;
int spawnTimer = 0;


State::State(int s, int b, int l) {
	scores = s;
	hp = b;
	level = l;
}

State init() {

	fighter_x = height / 2, fighter_y = width / 2;
	planeMatrix[fighter_x][fighter_y] = 1;
	State s = State(0, 3, 1);
	return s;
}


State drawPicture(State s) {
	//画图前先判断是被击中
	s = CollisionCheck(s);

	s = setEnemys(s);

	for (int x = 0; x < height; x++) {
		for (int y = 0; y < width; y++) {
			if (planeMatrix[x][y] == 1) {
				cout << "*";
			}
			else if (planeMatrix[x][y] == 2) {
				cout << "@";
			}
			else if (planeMatrix[x][y] == 3) {
				cout << "|";
			}
			else if(x == 0 || x == height-1 || y == 0 || y == width-1 ){
				cout << "#";
			}
			else {
				cout << " ";
			}
		}
		cout << endl;
	}
	cout << "| Hp: " << s.hp << " | Scores:" << s.scores  << "/" << s.level * 3 << " | Level:" << s.level << " |"<< endl
		<< "wsad控制飞机移动，空格键发射子弹，按0退出游戏";
	return s;
}





State operateFighter(State s) {
	HideCursor();
	if (_kbhit()) {
		char input = _getch();
		//移动飞机
		switch (input)
		{
			// W 向上移动，行坐标减小
		case 'W':
		case 'w':
			//边界判定：不能小于0（地图最顶部）
			if (fighter_x > 1) {
				planeMatrix[fighter_x][fighter_y] = 0;
				fighter_x--;
			}
			break;

			// S 向下移动
		case 'S':
		case 's':
			if (fighter_x < height - 2) {
				planeMatrix[fighter_x][fighter_y] = 0;
				fighter_x++;
			}
			break;

			// A 向左移动
		case 'A':
		case 'a':
			if (fighter_y > 1) {
				planeMatrix[fighter_x][fighter_y] = 0;
				fighter_y--;
			}
			break;

			// D 向右移动
		case 'D':
		case 'd':
			if (fighter_y < width - 2) {
				planeMatrix[fighter_x][fighter_y] = 0;
				fighter_y++;
			}
			break;
		case ' ':
			if (fighter_x > 1) {
				bullet_x = fighter_x - 1;
				bullet_y = fighter_y;
				planeMatrix[bullet_x][bullet_y] = 3;
			}
			break;
		case '0':
			s.level = -1;
			break;
		}
	}
	return s;
}


State pcOperate(State s) {
	floatBullet();
	s = enemyFighter(s);
	return s;
}



void floatBullet() {
	if (bullet_x > 0) {
		planeMatrix[bullet_x][bullet_y] = 0;
		bullet_x--;
	}
	if (bullet_x == 0) {
		planeMatrix[bullet_x][bullet_y] = 0;
	}
}



State enemyFighter(State s) {

	//逐个生成移动敌机
	if (activeEnemyCount < s.level * s.level) {
		spawnTimer++;
		if (spawnTimer >= 30) {
			resetEnemy(activeEnemyCount);
			activeEnemyCount++;
			spawnTimer = 0;
		}
	}

	if (speed < (30 / s.level)) {
		speed++;
	}
	if (speed >= (30 / s.level)) {
		for (int i = 0; i < activeEnemyCount; i++) {
			planeMatrix[enemy_x[i]][enemy_y[i]] = 0;
			enemy_x[i]++;
			planeMatrix[enemy_x[i]][enemy_y[i]] = 2;
		}
		speed = 0;
	}

	for (int i = 0; i < activeEnemyCount; i++) {
		if (enemy_x[i] > height - 3) {
			resetEnemy(i);
		}
	}
	return s;
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

State isWin(State s) {
	if (s.scores == s.level * 3) {
		if (s.level > 0 && s.level < 3) {
			s.hp++;
			s.level++;
			s.scores = 0;
			s.initial = true;
			memset(planeMatrix, 0, sizeof(planeMatrix));
			speed = 0;
			system("cls");
			cout << "第" << s.level << "关" << endl;
			system("pause");
		}
		else {
			s.level = 0;
			system("cls");
			cout << "恭喜你！已通过所有关卡！" << endl;
		}

	}
	if (s.hp == 0) {
		system("cls");
		cout << "很遗憾，闯关失败！" << endl;
	}
	if (s.level == -1){
		system("cls");
		cout << "已退出游戏" << endl;
	}
	return s;
}

State CollisionCheck(State s) {

	//碰到敌机扣生命值
	if (planeMatrix[fighter_x][fighter_y] == 2) {
		s.hp--;
		int old_fx = fighter_x, old_fy = fighter_y;
		planeMatrix[fighter_x][fighter_y] = 0;
		fighter_x = height / 2;
		fighter_y = width / 2;
		planeMatrix[fighter_x][fighter_y] = 1;
		for (int i = 0; i < activeEnemyCount; i++) {
			if (old_fx == enemy_x[i] && old_fy == enemy_y[i]) {
				resetEnemy(i);
				break;
			}
		}
	}
	else {
		planeMatrix[fighter_x][fighter_y] = 1;
	}

	//打中敌机加分
	if (planeMatrix[bullet_x][bullet_y] == 2) {
		s.scores++;
		planeMatrix[bullet_x][bullet_y] = 0;
		for (int i = 0; i < activeEnemyCount; i++) {
			if (bullet_x == enemy_x[i] && bullet_y == enemy_y[i]) {
				resetEnemy(i);
				break;
			}
		}
	}
	else if (bullet_x > 0) {
		planeMatrix[bullet_x][bullet_y] = 3;
	}
	return s;
}

State setEnemys(State s) {
	if (s.initial) {
		//放置静止敌机
		for (int k = 0; k < 2 + s.level; k++)
		{
			int rx, ry;
			do
			{
				rx = rand() % (height-1)+1;
				ry = rand() % (width-1)+1;
			} while ((rx == fighter_x && ry == fighter_y) || planeMatrix[rx][ry] == 2);

			planeMatrix[rx][ry] = 2;
		}
		//初始化移动敌机生成状态
		activeEnemyCount = 0;
		spawnTimer = 0;
		s.initial = false;
	}
	return s;
}

void resetEnemy(int index) {
	planeMatrix[enemy_x[index]][enemy_y[index]] = 0;
	do {
		enemy_x[index] = 1;
		enemy_y[index] = rand() % (width - 2);
	} while (isCover(enemy_y[index]));
	planeMatrix[enemy_x[index]][enemy_y[index]] = 2;
}

bool isCover(int y) {
	bool cover = false;
	for (int i = 1; i < height; i++) {
		if (planeMatrix[i][y] == 2) {
			cover = true;
		}
	}
	return cover;
}
