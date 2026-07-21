#include "MazeGame.h"

// 全局变量定义（extern 声明在 common.h 中，此处为实际分配内存）
int g_cellSize = 16;
int g_offsetX  = 0;
int g_offsetY  = 0;

int main() {
    MazeGame game;
    game.run();
    return 0;
}
