#pragma once
#include "common.h"
#include "Point.h"

// 迷宫类：管理迷宫地图的存储、DFS 生成、图形绘制
class Maze {
public:
    // 初始化为 rows×cols 全墙迷宫（行列须为奇数），起点 (1,1)，终点 (cols-2,rows-2)
    void init(int rows, int cols);
    // 从起点开始 DFS 递归挖出通路
    void generate();
    // 逐格绘制迷宫：墙壁=黑色，通路=白色，终点=绿色
    void draw() const;

    // 查询接口：越界返回墙壁
    int  getCell(int x, int y) const;
    int  getRows() const;
    int  getCols() const;
    Point getStartPos() const;
    Point getEndPos() const;

private:
    int m_rows, m_cols;                       // 迷宫行列数
    vector<vector<int>> m_mazeMap;            // 二维地图：m_mazeMap[y][x]
    Point m_startPos, m_endPos;               // 起点、终点坐标

    // DFS 隔格探索方向：步长为 2，跳过一格（那格是待凿的墙）
    const int m_genDirs[4][2] = {{0,-2},{0,2},{-2,0},{2,0}};

    void generate(int x, int y);              // DFS 递归实现
    void shuffleDirections(vector<int>& dirIndex) const;  // Fisher-Yates 洗牌
};
