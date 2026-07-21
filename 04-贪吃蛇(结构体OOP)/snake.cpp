#include "snake.h"

void Snake::init(int startX, int startY, int length, int dir) {
	body.clear();
	direction = dir;
	// 蛇身水平向左延伸：body[0]=头, body[1..len-1]=身
	for (int i = 0; i < length; i++) {
		body.push_back({ startX, startY - i });
	}
}

Point Snake::nextHead() const {
	Point head = body[0];
	switch (direction) {
	case DIR_UP:    head.x--; break;
	case DIR_DOWN:  head.x++; break;
	case DIR_LEFT:  head.y--; break;
	case DIR_RIGHT: head.y++; break;
	}
	return head;
}

void Snake::move(Point newHead, bool grow) {
	// 头部前插（newHead 已由调用方 wrap 过）
	body.push_front(newHead);
	// 没吃到食物就删尾
	if (!grow) {
		body.pop_back();
	}
}

bool Snake::occupies(int x, int y) const {
	for (const auto& p : body) {
		if (p.x == x && p.y == y) return true;
	}
	return false;
}

int Snake::size() const {
	return (int)body.size();
}
