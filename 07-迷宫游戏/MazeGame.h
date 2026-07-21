#pragma once
#include "common.h"
#include "Maze.h"
#include "Player.h"
#include "PathFinder.h"

// 游戏主控类：聚合迷宫、玩家、寻路器，管理整个游戏流程
class MazeGame {
public:
    void run();  // 启动游戏主循环

private:
    Maze       m_maze;          // 迷宫
    Player     m_player;        // 玩家
    PathFinder m_pathFinder;    // 寻路器

    int  m_currentLevel  = 0;   // 当前关卡 (0~4)
    int  m_stepCount     = 0;   // 步数
    bool m_showPath      = false;  // 是否显示最短路径
    int  m_offsetX       = 0;      // 迷宫水平偏移
    int  m_offsetY       = 0;      // 迷宫垂直偏移

    static const int WIN_W = 660;  // 固定窗口宽度
    static const int WIN_H = 700;  // 固定窗口高度

    // 关卡配置：5 个递增难度，行列均为奇数
    const int m_levelConfig[5][2] = {
        {11,13}, {15,17}, {19,19}, {23,23}, {31,31}
    };

    void initLevel();              // 初始化当前关卡
    void handleInput();            // 处理键盘输入
    void autoWalkAnimation();      // 自动寻路动画
    void updatePath();             // 从玩家当前位置 BFS 到终点
    void drawStatusBar() const;    // 绘制顶部状态栏
    void renderFrame();            // 渲染一帧画面
    void handleLevelComplete();    // 通关处理
};
