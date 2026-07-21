#include "game.h"
#include <iostream>
#include <conio.h>
#include <Windows.h>
using namespace std;

void Game::init() {
	m_running = true;

	// 初始化画面矩阵
	for (int x = 0; x < HEIGHT; x++) {
		for (int y = 0; y < WIDTH; y++) {
			m_board[x][y] = 0;
		}
	}

	// 边框
	for (int i = 0; i < HEIGHT; i++) {
		m_board[i][0] = -1;
		m_board[i][WIDTH - 1] = -1;
	}
	for (int j = 0; j < WIDTH; j++) {
		m_board[0][j] = -1;
		m_board[HEIGHT - 1][j] = -1;
	}

	// 初始化蛇：位置(HEIGHT/2, WIDTH/2)，长度5，方向右
	m_snake.init(HEIGHT / 2, WIDTH / 2, 5, DIR_RIGHT);

	// 生成食物
	m_food.spawn(m_snake);

	hideCursor();
}

void Game::updateBoard() {
	// 清空可玩区域
	for (int x = 1; x < HEIGHT - 1; x++) {
		for (int y = 1; y < WIDTH - 1; y++) {
			m_board[x][y] = 0;
		}
	}

	// 放置蛇身（头='@' 用值1，身='*' 用值2）
	const auto& body = m_snake.body;
	m_board[body[0].x][body[0].y] = 1;            // 头
	for (size_t i = 1; i < body.size(); i++) {
		m_board[body[i].x][body[i].y] = 2;         // 身
	}

	// 放置食物
	m_board[m_food.x][m_food.y] = -2;
}

void Game::draw() {
	gotoxy(0, 0);
	for (int x = 0; x < HEIGHT; x++) {
		for (int y = 0; y < WIDTH; y++) {
			switch (m_board[x][y]) {
			case  0: cout << ' '; break;   // 空地
			case -1: cout << '#'; break;   // 墙
			case  1: cout << '@'; break;   // 蛇头
			case  2: cout << '*'; break;   // 蛇身
			case -2: cout << 'F'; break;   // 食物
			}
		}
		cout << endl;
	}
}

void Game::tick() {
	Point next = m_snake.nextHead();

	// 穿越边界
	wrapPoint(next);

	// 撞到自己 → 游戏结束
	// 注意：此处蛇尾尚未移走，若蛇头走到当前蛇尾的位置也会判碰撞
	// 这与原版行为一致（原版 isCover 检查时蛇尾也未清除）
	if (m_snake.occupies(next.x, next.y)) {
		checkWin();  // 满了算赢
		system("cls");
		cout << "游戏失败！" << endl;
		Sleep(2000);
		exit(0);
	}

	// 判断是否吃到食物
	bool ate = (next.x == m_food.x && next.y == m_food.y);

	m_snake.move(next, ate);

	if (ate) {
		m_food.spawn(m_snake);
	}
}

void Game::wrapPoint(Point& p) const {
	if (p.x < 1)           p.x = HEIGHT - 2;
	if (p.x > HEIGHT - 2)  p.x = 1;
	if (p.y < 1)           p.y = WIDTH - 2;
	if (p.y > WIDTH - 2)   p.y = 1;
}

void Game::checkWin() {
	int total = (HEIGHT - 2) * (WIDTH - 2);
	if (m_snake.size() >= total) {
		system("cls");
		cout << "恭喜通关！" << endl;
		Sleep(2000);
		exit(0);
	}
}

void Game::gotoxy(int x, int y) const {
	COORD pos = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void Game::hideCursor() const {
	CONSOLE_CURSOR_INFO info = { 1, FALSE };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}

void Game::run() {
	init();
	while (m_running) {
		// 同方向加速：按键匹配当前方向 → 一帧走 4 步
		bool boost = false;
		if (_kbhit()) {
			char ch = _getch();

			// 同方向加速
			if ((ch == 'w' || ch == 'W') && m_snake.direction == DIR_UP)    boost = true;
			if ((ch == 's' || ch == 'S') && m_snake.direction == DIR_DOWN)  boost = true;
			if ((ch == 'a' || ch == 'A') && m_snake.direction == DIR_LEFT)  boost = true;
			if ((ch == 'd' || ch == 'D') && m_snake.direction == DIR_RIGHT) boost = true;

			// 不同方向 → 转向（反向会被 tick() 中 occupies() 捕获为撞身）
			if (!boost) {
				switch (ch) {
				case 'w': case 'W': m_snake.direction = DIR_UP;    break;
				case 's': case 'S': m_snake.direction = DIR_DOWN;  break;
				case 'a': case 'A': m_snake.direction = DIR_LEFT;  break;
				case 'd': case 'D': m_snake.direction = DIR_RIGHT; break;
				}
			}
		}

		// 加速时连续走多步，每步都画出来
		int steps = boost ? 4 : 1;
		int delay = boost ? 50 : 200;
		for (int i = 0; i < steps; i++) {
			tick();
			updateBoard();
			draw();
			Sleep(delay);
		}
	}
}
