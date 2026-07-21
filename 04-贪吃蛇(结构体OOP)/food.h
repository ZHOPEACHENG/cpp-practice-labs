#pragma once

#include "common.h"

struct Snake;  // 前向声明

// 食物结构体
struct Food {
	int x, y;

	// 在空白位置随机生成食物
	void spawn(const Snake& snake);
};
