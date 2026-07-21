#include "snake.h"
#include <iostream>
#include <conio.h>
#include <Windows.h>
using namespace std;

// 全局游戏状态
static Snake g_snake;
static Food  g_food;
static int   g_board[HEIGHT][WIDTH];
static int   g_prevDirection = DIR_RIGHT;  // 用于碰撞时回退方向

// 食物生成
static void foodSpawn(Food& food, const Snake& snake) {
	int totalCells = (HEIGHT - 2) * (WIDTH - 2);
	if (snake.length >= totalCells) return;
	do {
		food.x = rand() % (HEIGHT - 2) + 1;
		food.y = rand() % (WIDTH - 2) + 1;
	} while (snakeOccupies(snake, food.x, food.y));
}

// 工具函数
static void wrapPoint(int& x, int& y) {
	if (x < 1)           x = HEIGHT - 2;
	if (x > HEIGHT - 2)  x = 1;
	if (y < 1)           y = WIDTH - 2;
	if (y > WIDTH - 2)   y = 1;
}

static void gotoxy(int x, int y) {
	COORD pos = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

static void hideCursor() {
	CONSOLE_CURSOR_INFO info = { 1, FALSE };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}

// 结束判定
static bool isBoardFull() {
	return g_snake.length >= (HEIGHT - 2) * (WIDTH - 2);
}

static void endGame(bool win) {
	system("cls");
	if (win) {
		cout << "           恭喜通关！满屏了！" << endl;
	}
	else {
		int s = g_snake.score;
		if (s >= 30) {
			cout << "\t\t太强了！你是蛇中之王！  (" << s << " 分)" << endl;
		} else if (s >= 20) {
			cout << "\t\t不错！颇有蛇者风范！  (" << s << " 分)" << endl;
		} else if (s >= 10) {
			cout << "\t\t还行，再接再厉！  (" << s << " 分)" << endl;
		} else if (s > 0) {
			cout << "\t\t刚起步就没了？ (" << s << " 分)" << endl;
		} else {
			cout << "\t\t是不爱吃饭吗？... (" << s << " 分)" << endl;
		}
	}
	Sleep(3000);
	exit(0);
}

// 蛇身闪动
static void flashSnake() {
	// 记录蛇身所有坐标（从旧 body）
	int total = g_snake.length;
	int* xs = new int[total];
	int* ys = new int[total];
	int idx = 0;
	for (Node* p = g_snake.head; p != nullptr; p = p->next) {
		xs[idx] = p->x;
		ys[idx] = p->y;
		idx++;
	}

	// 闪两下：清除 → 画 → 清除 → 画
	for (int flash = 0; flash < 2; flash++) {
		// 清除蛇身（画空格）
		for (int i = 0; i < total; i++) {
			gotoxy(ys[i], xs[i]);
			cout << ' ';
		}
		Sleep(150);

		// 恢复蛇身
		gotoxy(ys[0], xs[0]);
		cout << '@';
		for (int i = 1; i < total; i++) {
			gotoxy(ys[i], xs[i]);
			cout << '*';
		}
		Sleep(150);
	}

	delete[] xs;
	delete[] ys;
}

// 核心逻辑
static void initGame() {
	system("cls");
	for (int x = 0; x < HEIGHT; x++)
		for (int y = 0; y < WIDTH; y++)
			g_board[x][y] = 0;

	// 边框
	for (int i = 0; i < HEIGHT; i++) {
		g_board[i][0] = -1;
		g_board[i][WIDTH - 1] = -1;
	}
	for (int j = 0; j < WIDTH; j++) {
		g_board[0][j] = -1;
		g_board[HEIGHT - 1][j] = -1;
	}

	snakeInit(g_snake, HEIGHT / 2, WIDTH / 2, 5, DIR_RIGHT);
	foodSpawn(g_food, g_snake);
	hideCursor();
}

static void updateBoard() {
	for (int x = 1; x < HEIGHT - 1; x++)
		for (int y = 1; y < WIDTH - 1; y++)
			g_board[x][y] = 0;

	g_board[g_snake.head->x][g_snake.head->y] = 1;
	for (Node* p = g_snake.head->next; p != nullptr; p = p->next)
		g_board[p->x][p->y] = 2;

	g_board[g_food.x][g_food.y] = -2;
}

static void drawBoard() {
	gotoxy(0, 0);
	for (int x = 0; x < HEIGHT; x++) {
		for (int y = 0; y < WIDTH; y++) {
			switch (g_board[x][y]) {
			case  0: cout << ' '; break;
			case -1: cout << '#'; break;
			case  1: cout << '@'; break;
			case  2: cout << '*'; break;
			case -2: cout << 'F'; break;
			}
		}
		cout << endl;
	}
	// HUD：生命值 + 得分
	cout << "  Life: " << g_snake.lives;
	cout << "   Score: " << g_snake.score << "  " << endl;
}

static void tick() {
	int nx, ny;
	snakeNextHead(g_snake, nx, ny);
	wrapPoint(nx, ny);

	// 碰撞 → 扣命，回退方向，原地继续
	if (snakeOccupies(g_snake, nx, ny)) {
		flashSnake();
		g_snake.lives--;
		g_snake.direction = g_prevDirection;  // 撤销导致碰撞的方向变更

		if (g_snake.lives <= 0) {
			system("cls");
			endGame(false);
		}
		return;
	}

	// 满屏 → 胜利
	if (isBoardFull()) {
		endGame(true);
	}

	bool ate = (nx == g_food.x && ny == g_food.y);
	snakeMove(g_snake, nx, ny, ate);
	if (ate) {
		foodSpawn(g_food, g_snake);
	}
}

// 主循环
void runGame() {
	initGame();
	while (true) {
		g_prevDirection = g_snake.direction;  // 保存安全方向
		bool boost = false;
		if (_kbhit()) {
			char ch = _getch();

			if ((ch == 'w' || ch == 'W') && g_snake.direction == DIR_UP)    boost = true;
			if ((ch == 's' || ch == 'S') && g_snake.direction == DIR_DOWN)  boost = true;
			if ((ch == 'a' || ch == 'A') && g_snake.direction == DIR_LEFT)  boost = true;
			if ((ch == 'd' || ch == 'D') && g_snake.direction == DIR_RIGHT) boost = true;

			if (!boost) {
				switch (ch) {
				case 'w': case 'W': g_snake.direction = DIR_UP;    break;
				case 's': case 'S': g_snake.direction = DIR_DOWN;  break;
				case 'a': case 'A': g_snake.direction = DIR_LEFT;  break;
				case 'd': case 'D': g_snake.direction = DIR_RIGHT; break;
				}
			}
		}

		int steps = boost ? 4 : 1;
		int delay = boost ? 50 : 200;
		for (int i = 0; i < steps; i++) {
			tick();
			updateBoard();
			drawBoard();
			Sleep(delay);
		}
	}
}
