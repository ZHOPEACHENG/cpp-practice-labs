#include "snake.h"
#include <cstdlib>

void snakeInit(Snake& snake, int startX, int startY, int length, int dir) {
	snake.direction = dir;
	snake.length = length;
	snake.lives = 3;
	snake.score = 0;
	snake.head = nullptr;
	snake.tail = nullptr;

	// 头插法：从尾到头创建节点，最后插入的成为蛇头
	for (int i = 0; i < length; i++) {
		Node* node = new Node{ startX, startY - length + 1 + i, nullptr, nullptr };
		if (snake.head == nullptr) {
			snake.head = node;
			snake.tail = node;
		} else {
			node->next = snake.head;
			snake.head->prev = node;
			snake.head = node;
		}
	}
}

void snakeDestroy(Snake& snake) {
	Node* p = snake.head;
	while (p != nullptr) {
		Node* next = p->next;
		delete p;
		p = next;
	}
	snake.head = nullptr;
	snake.tail = nullptr;
	snake.length = 0;
}

void snakeRespawn(Snake& snake, int startX, int startY, int length, int dir) {
	snakeDestroy(snake);
	snake.direction = dir;
	snake.length = length;
	// lives 和 score 保留不重置

	for (int i = 0; i < length; i++) {
		Node* node = new Node{ startX, startY - length + 1 + i, nullptr, nullptr };
		if (snake.head == nullptr) {
			snake.head = node;
			snake.tail = node;
		} else {
			node->next = snake.head;
			snake.head->prev = node;
			snake.head = node;
		}
	}
}

void snakeNextHead(const Snake& snake, int& nx, int& ny) {
	nx = snake.head->x;
	ny = snake.head->y;
	switch (snake.direction) {
	case DIR_UP:    nx--; break;
	case DIR_DOWN:  nx++; break;
	case DIR_LEFT:  ny--; break;
	case DIR_RIGHT: ny++; break;
	}
}

void snakeMove(Snake& snake, int newX, int newY, bool grow) {
	Node* newHead = new Node{ newX, newY, snake.head, nullptr };
	snake.head->prev = newHead;
	snake.head = newHead;

	if (!grow) {
		Node* oldTail = snake.tail;
		snake.tail = oldTail->prev;
		snake.tail->next = nullptr;
		delete oldTail;
	} else {
		snake.length++;
		snake.score += 10;  // 吃到一个食物 +10 分
	}
}

bool snakeOccupies(const Snake& snake, int x, int y) {
	for (Node* p = snake.head; p != nullptr; p = p->next) {
		if (p->x == x && p->y == y) return true;
	}
	return false;
}
