#pragma once
#include "common.h"
#include "Point.h"
#include "Maze.h"

// 寻路类：BFS 广度优先搜索最短路径
class PathFinder {
public:
    // BFS 搜索从 start 到 end 的最短路径
    void bfsSearch(const Maze& maze, Point start, Point end);
    const vector<Point>& getPath() const;  // 获取搜索结果
    void drawPath() const;                 // 绘制路径（蓝色方块）

private:
    vector<Point> m_path;                  // 路径点序列

    // 四方向偏移：步长为 1（上下左右走一格）
    const int m_moveDirs[4][2] = {{0,-1},{0,1},{-1,0},{1,0}};
};
