#include "Player.h"

// 构造：加载角色图片到 48×48 参考尺寸，绘制时拉伸适配实际格子
Player::Player() : m_pos(1, 1), m_hasImage(false) {
    m_hasImage = (loadimage(&m_gamerImg, _T("res/gamer.png"), 32, 32, true) == S_OK);
}

// 初始化：放置到迷宫起点
void Player::init(const Maze& maze) { m_pos = maze.getStartPos(); }

// 移动：目标为通路则走，撞墙返回 false
bool Player::move(Direction dir, const Maze& maze) {
    int dx = 0, dy = 0;
    switch (dir) {
        case DIR_UP:    dy = -1; break;
        case DIR_DOWN:  dy =  1; break;
        case DIR_LEFT:  dx = -1; break;
        case DIR_RIGHT: dx =  1; break;
    }
    if (maze.getCell(m_pos.x + dx, m_pos.y + dy) == CELL_PATH) {
        m_pos.x += dx;
        m_pos.y += dy;
        return true;
    }
    return false;
}

// 直接设坐标（自动寻路用）
void Player::setPos(const Point& pos) { m_pos = pos; }

// 绘制：有图片则拉伸绘制，无图片用黄色方块代替
void Player::draw() const {
    int px = g_offsetX + m_pos.x * g_cellSize;
    int py = m_pos.y * g_cellSize + g_offsetY;

    if (m_hasImage) {
        putimage(px, py, g_cellSize, g_cellSize, &m_gamerImg, 0, 0);
    } else {
        setfillcolor(CLR_PLAYER);
        solidrectangle(px + 5, py + 5, px + g_cellSize - 5, py + g_cellSize - 5);
    }
}

Point Player::getPos() const { return m_pos; }

bool Player::isAtEnd(const Maze& maze) const {
    return m_pos == maze.getEndPos();
}
