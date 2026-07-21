#pragma once

#include "snake.h"
#include "food.h"

// 游戏控制类 —— 整合蛇和食物，管理游戏流程
class Game {
public:
	void init();
	void run();

private:
	Snake m_snake;
	Food  m_food;
	bool  m_running;              // 游戏是否运行中
	int   m_board[HEIGHT][WIDTH]; // 画面矩阵

	void updateBoard();           // 刷新画面矩阵
	void draw();                  // 打印到控制台
	void tick();                  // 每帧逻辑：移动 + 碰撞 + 食物
	void wrapPoint(Point& p) const; // 穿越边界处理
	void gotoxy(int x, int y) const;
	void hideCursor() const;
	void checkWin();              // 胜利判定
};
