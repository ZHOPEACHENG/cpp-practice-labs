#include "PathFinder.h"

// BFS 最短路径搜索：
// visit 标记已访问，pre 记录前驱，到达终点后回溯 + 反转得到正向路径
void PathFinder::bfsSearch(const Maze& maze, Point start, Point end) {
    m_path.clear();
    int rows = maze.getRows(), cols = maze.getCols();

    vector<vector<bool>> visit(rows, vector<bool>(cols, false));
    vector<vector<Point>> pre(rows, vector<Point>(cols, Point(-1, -1)));
    queue<Point> q;

    q.push(start);
    visit[start.y][start.x] = true;

    while (!q.empty()) {
        Point cur = q.front(); 
        q.pop();

        if (cur == end) {
            for (Point p = end; p.x != -1; p = pre[p.y][p.x])
                m_path.push_back(p);
            reverse(m_path.begin(), m_path.end());
            return;
        }

        for (int i = 0; i < 4; i++) {
            int nx = cur.x + m_moveDirs[i][0];
            int ny = cur.y + m_moveDirs[i][1];
            if (nx >= 0 && nx < cols && ny >= 0 && ny < rows &&
                !visit[ny][nx] && maze.getCell(nx, ny) == CELL_PATH) {
                visit[ny][nx] = true;
                pre[ny][nx] = cur;
                q.push(Point(nx, ny));
            }
        }
    }
}

const vector<Point>& PathFinder::getPath() const { return m_path; }

// 绘制路径：跳过首尾（起点是玩家，终点已有标记）
void PathFinder::drawPath() const {
    setfillcolor(CLR_BFS);
    for (size_t i = 1; i + 1 < m_path.size(); i++) {
        int px = g_offsetX + m_path[i].x * g_cellSize;
        int py = m_path[i].y * g_cellSize + g_offsetY;
        int mgn = max(2, g_cellSize / 5);
        solidrectangle(px + mgn, py + mgn, px + g_cellSize - mgn, py + g_cellSize - mgn);
    }
}
