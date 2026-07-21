#pragma once
#include "common.h"
#include "Point.h"
#include "Maze.h"

// 玩家类：管理玩家位置、移动判定、角色绘制
class Player {
public:
    Player();  // 加载角色图片

    void init(const Maze& maze);                 // 放置到迷宫起点
    bool move(Direction dir, const Maze& maze);  // 移动，撞墙返回 false
    void setPos(const Point& pos);               // 直接设置坐标（自动寻路用）
    void draw() const;                           // 绘制角色（图片或黄色方块）

    Point getPos() const;
    bool  isAtEnd(const Maze& maze) const;       // 是否到达终点

private:
    Point m_pos;          // 当前格子坐标
    IMAGE m_gamerImg;     // 角色图片
    bool  m_hasImage;     // 图片是否加载成功
};
