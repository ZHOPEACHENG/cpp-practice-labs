#pragma once

#include <deque>
#include "common.h"

// 坐标点
struct Point {
	int x, y;
};

// 蛇结构体
struct Snake {
	std::deque<Point> body;   // body[0] 是蛇头
	int direction;             // 当前移动方向

	// 初始化蛇：指定起始位置
	void init(int startX, int startY, int length, int dir);

	// 计算下一步蛇头坐标（原始坐标，未 wrap）
	Point nextHead() const;

	// 移动一格：在 newHead 处放新头，grow=true 时保留尾巴（变长）
	// newHead 应由调用方提前 wrap
	void move(Point newHead, bool grow);

	// 某个坐标是否被蛇身占据（碰撞检测 + 食物生成共用）
	bool occupies(int x, int y) const;

	// 蛇身覆盖了多少格（用于胜利判定）
	int size() const;
};
