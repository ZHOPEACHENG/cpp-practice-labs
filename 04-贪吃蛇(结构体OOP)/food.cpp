#include "food.h"
#include "snake.h"
#include <cstdlib>

void Food::spawn(const Snake& snake) {
	// 可玩区域: x∈[1, HEIGHT-2], y∈[1, WIDTH-2]
	int totalCells = (HEIGHT - 2) * (WIDTH - 2);
	if (snake.size() >= totalCells) {
		return; // 满了，不生成
	}
	do {
		x = rand() % (HEIGHT - 2) + 1;
		y = rand() % (WIDTH - 2) + 1;
	} while (snake.occupies(x, y));
}
