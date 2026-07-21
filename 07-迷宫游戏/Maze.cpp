#include "Maze.h"

// 初始化：全部格子设为墙壁，设置起点和终点
void Maze::init(int rows, int cols) {
    m_rows = rows;
    m_cols = cols;
    m_mazeMap.assign(rows, vector<int>(cols, CELL_WALL));
    m_startPos = Point(1, 1);
    m_endPos   = Point(cols - 2, rows - 2);
}

// 生成迷宫入口：从起点开始递归
void Maze::generate() {
    generate(m_startPos.x, m_startPos.y);
}

// DFS 递归挖迷宫：
// 1. 当前位置设为通路
// 2. 随机打乱四个方向顺序
// 3. 逐个方向尝试：隔一格探索，若目标是墙则凿穿中间格，递归进入
// 步长为 2 保证墙和路交替出现，形成迷宫结构
void Maze::generate(int x, int y) {
    m_mazeMap[y][x] = CELL_PATH;                // 当前位置变通路

    vector<int> dirIndex = {0, 1, 2, 3};        // 四个方向索引
    shuffleDirections(dirIndex);                 // 随机打乱

    for (int i = 0; i < 4; i++) {
        int idx  = dirIndex[i];
        int nextX = x + m_genDirs[idx][0];       // 隔一格的目标坐标
        int nextY = y + m_genDirs[idx][1];

        // 目标在边界内、且未被访问过（仍是墙）
        if (nextX > 0 && nextX < m_cols - 1 &&
            nextY > 0 && nextY < m_rows - 1 &&
            m_mazeMap[nextY][nextX] == CELL_WALL) {

            // 凿穿当前位置与目标位置之间的隔墙
            int midX = x + m_genDirs[idx][0] / 2;
            int midY = y + m_genDirs[idx][1] / 2;
            m_mazeMap[midY][midX] = CELL_PATH;

            generate(nextX, nextY);              // 递归进入新格子
        }
    }
}

// 洗牌：保证四个方向的 24 种排列概率均等
void Maze::shuffleDirections(vector<int>& dirIndex) const {
    for (int i = 3; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(dirIndex[i], dirIndex[j]);
    }
}

// 逐格绘制迷宫：墙壁=黑色，通路=白色，终点=绿色
void Maze::draw() const {
    for (int y = 0; y < m_rows; y++) {
        for (int x = 0; x < m_cols; x++) {
            int px = g_offsetX + x * g_cellSize;
            int py = y * g_cellSize + g_offsetY;
            setfillcolor(m_mazeMap[y][x] == CELL_WALL ? CLR_WALL : CLR_PATH);
            solidrectangle(px, py, px + g_cellSize, py + g_cellSize);
        }
    }
    // 边框：墙与背景的过渡阴影（10px 厚）
    int bx = g_offsetX - 10;
    int by = g_offsetY - 10;
    int bw = m_cols * g_cellSize + 20;
    int bh = m_rows * g_cellSize + 20;
    setfillcolor(CLR_BORDER);
    solidrectangle(bx, by, bx + bw - 1, by + 9);           // 上
    solidrectangle(bx, by + bh - 10, bx + bw - 1, by + bh - 1); // 下
    solidrectangle(bx, by + 10, bx + 9, by + bh - 11);     // 左
    solidrectangle(bx + bw - 10, by + 10, bx + bw - 1, by + bh - 11); // 右

    // 终点标记（内缩，不占满格子）
    int ex = g_offsetX + m_endPos.x * g_cellSize;
    int ey = m_endPos.y * g_cellSize + g_offsetY;
    int mgn = max(2, g_cellSize / 5);
    setfillcolor(CLR_END);
    solidrectangle(ex + mgn, ey + mgn, ex + g_cellSize - mgn, ey + g_cellSize - mgn);
}

// 查询接口
int Maze::getCell(int x, int y) const {
    if (x < 0 || x >= m_cols || y < 0 || y >= m_rows)
        return CELL_WALL;  // 越界视为墙
    return m_mazeMap[y][x];
}
int Maze::getRows() const { return m_rows; }
int Maze::getCols() const { return m_cols; }
Point Maze::getStartPos() const { return m_startPos; }
Point Maze::getEndPos()   const { return m_endPos; }
