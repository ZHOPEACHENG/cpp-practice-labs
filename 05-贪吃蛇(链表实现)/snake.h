#pragma once


// 常量
constexpr int HEIGHT = 20;
constexpr int WIDTH  = 30;

constexpr int DIR_UP    = 1;
constexpr int DIR_DOWN  = 2;
constexpr int DIR_LEFT  = 3;
constexpr int DIR_RIGHT = 4;

// 双向链表节点
struct Node {
	int  x, y;
	Node* next;
	Node* prev;
};


// 蛇结构体 —— 身体用双向链表存储
struct Snake {
	Node* head;      // 链表头 = 蛇头
	Node* tail;      // 链表尾 = 蛇尾
	int   direction; // 当前方向
	int   length;    // 当前长度
	int   lives;     // 生命值
	int   score;     // 得分
};

void snakeInit(Snake& snake, int startX, int startY, int length, int dir);
void snakeDestroy(Snake& snake);
void snakeRespawn(Snake& snake, int startX, int startY, int length, int dir);
void snakeNextHead(const Snake& snake, int& nx, int& ny);
void snakeMove(Snake& snake, int newX, int newY, bool grow);
bool snakeOccupies(const Snake& snake, int x, int y);


// 食物结构体
struct Food {
	int x, y;
};

void foodSpawn(Food& food, const Snake& snake);
