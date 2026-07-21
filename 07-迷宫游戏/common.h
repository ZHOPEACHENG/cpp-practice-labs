#pragma once
// 公共头文件：所有源文件共用此文件中的库引用、全局变量、枚举类型

#include <graphics.h>   // EasyX 图形库（窗口、绘图）
#include <conio.h>      // _kbhit / _getch 键盘输入
#include <vector>       // STL 动态数组
#include <queue>        // STL 队列（BFS 用）
#include <cstdlib>      // rand / srand / exit
#include <ctime>        // time（随机种子）
#include <algorithm>    // reverse / swap / max / min

using namespace std;

// 全局变量：关卡间动态调整，所有绘制代码共用
extern int g_cellSize;          // 每个迷宫格子的像素大小
extern int g_offsetX;           // 迷宫水平偏移
extern int g_offsetY;           // 迷宫垂直偏移
const int STATUS_BAR_H = 30;    // 顶部状态栏高度（像素）

// 配色方案
const COLORREF CLR_WALL     = RGB( 20,  50,  80);
const COLORREF CLR_PATH     = RGB(250, 248, 244);
const COLORREF CLR_END      = RGB( 80, 240, 130);
const COLORREF CLR_BFS      = RGB(  0, 190, 200);
const COLORREF CLR_PLAYER   = RGB(255, 130,  70);
const COLORREF CLR_BORDER   = RGB( 40,  65,  95);
const COLORREF CLR_BG       = RGB( 36,  48,  62);
const COLORREF CLR_BAR_BG   = RGB( 15,  40,  65);
const COLORREF CLR_BAR_TEXT = RGB(238, 242, 245);

// 枚举
enum CellType { CELL_WALL = 1, CELL_PATH = 0 };  // 迷宫格子状态
enum Direction { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT };  // 移动方向
