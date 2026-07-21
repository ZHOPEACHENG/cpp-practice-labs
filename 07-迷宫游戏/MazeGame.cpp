#include "MazeGame.h"

// 游戏主循环
void MazeGame::run() {
    srand((unsigned int)time(NULL));          // 随机种子
    initgraph(WIN_W, WIN_H);                  // 创建固定窗口（只此一次，永不 close）
    SetWindowText(GetHWnd(), _T("迷宫游戏"));
    SetForegroundWindow(GetHWnd());           // 窗口置前

    initLevel();

    while (true) {
        if (m_player.isAtEnd(m_maze))
            handleLevelComplete();
        handleInput();                         // 键盘输入
        renderFrame();                         // 绘图
        Sleep(15);                             // ~66 FPS
    }
}

// 初始化当前关卡：计算格子大小、生成迷宫、放置玩家、预计算路径
void MazeGame::initLevel() {
    int rows = m_levelConfig[m_currentLevel][0];
    int cols = m_levelConfig[m_currentLevel][1];

    // 在固定窗口内尽量放大迷宫，格子大小限制在 [16, 40] 像素
    g_cellSize = max(16, min(40,
        min((WIN_W - 20) / cols, (WIN_H - STATUS_BAR_H - 20) / rows)));
    m_offsetX = (WIN_W - cols * g_cellSize) / 2;
    m_offsetY = STATUS_BAR_H + (WIN_H - STATUS_BAR_H - rows * g_cellSize) / 2;

    m_maze.init(rows, cols);
    m_maze.generate();
    m_player.init(m_maze);
    m_pathFinder.bfsSearch(m_maze, m_maze.getStartPos(), m_maze.getEndPos());

    m_stepCount = 0;
    m_showPath  = false;
}

// 渲染一帧：背景 → 迷宫 → 路径 → 玩家 → 状态栏
void MazeGame::renderFrame() {
    BeginBatchDraw();                         // 双缓冲开始

    setfillcolor(CLR_BG);
    solidrectangle(0, 0, WIN_W - 1, WIN_H - 1);  // 背景

    g_offsetX = m_offsetX;
    g_offsetY = m_offsetY;
    m_maze.draw();
    if (m_showPath) m_pathFinder.drawPath();
    m_player.draw();

    drawStatusBar();

    EndBatchDraw();
}

// 键盘输入：WASD 移动 / 空格自动寻路 / P 切换路径 / ESC 退出
void MazeGame::handleInput() {
    if (_kbhit()) {
        char key = _getch();
        switch (key) {
            case 'w': case 'W':
                if (m_player.move(DIR_UP,    m_maze)) { m_stepCount++; updatePath(); } break;
            case 's': case 'S':
                if (m_player.move(DIR_DOWN,  m_maze)) { m_stepCount++; updatePath(); } break;
            case 'a': case 'A':
                if (m_player.move(DIR_LEFT,  m_maze)) { m_stepCount++; updatePath(); } break;
            case 'd': case 'D':
                if (m_player.move(DIR_RIGHT, m_maze)) { m_stepCount++; updatePath(); } break;

            case ' ':                           // 空格 — 从当前位置自动走到终点
                m_showPath = true;
                updatePath();
                autoWalkAnimation();
                break;

            case 'p': case 'P':                 // P — 切换路径显示
                updatePath();
                m_showPath = !m_showPath;
                break;

            case 27:                            // ESC — 退出
                closegraph(); exit(0);
        }
    }
}

// 从玩家当前位置 BFS 到终点，更新路径
void MazeGame::updatePath() {
    m_pathFinder.bfsSearch(m_maze, m_player.getPos(), m_maze.getEndPos());
}

// 自动寻路：沿 BFS 路径逐格走到终点
void MazeGame::autoWalkAnimation() {
    const vector<Point>& path = m_pathFinder.getPath();
    if (path.size() <= 1) return;

    for (size_t i = 1; i < path.size(); i++) {
        if (_kbhit()) {
            char key = _getch();
            if (key == 27) { closegraph(); exit(0); }
            if (key == 'w' || key == 'W' || key == 'a' || key == 'A' ||
                key == 's' || key == 'S' || key == 'd' || key == 'D' ||
                key == ' ') {
                updatePath();
                break;
            }
            if(key == 'p' || key == 'P') {
                m_showPath = !m_showPath;
			}
        }
        m_player.setPos(path[i]);
        m_stepCount++;
        renderFrame();
        Sleep(100);
        if (m_player.isAtEnd(m_maze)) break;
    }
}

// 顶部状态栏：关卡、步数、操作提示
void MazeGame::drawStatusBar() const {
    setfillcolor(CLR_BAR_BG);
    solidrectangle(0, 0, WIN_W, STATUS_BAR_H);

    settextcolor(CLR_BAR_TEXT);
    setbkmode(TRANSPARENT);
    settextstyle(14, 0, _T("宋体"));

    TCHAR buf[256];
    _stprintf_s(buf, _T("第%d关 | 步数:%d | 空格=自动寻路 P=路径 WASD=移动 ESC=退出"),
                m_currentLevel + 1, m_stepCount);
    outtextxy(10, 8, buf);
}

// 通关弹窗 → 下一关 / 全部通关
void MazeGame::handleLevelComplete() {
    TCHAR buf[128];
    _stprintf_s(buf, _T("第%d关通关！\n总步数：%d"),
                m_currentLevel + 1, m_stepCount);
    MessageBox(GetHWnd(), buf, _T("恭喜通关"), MB_OK | MB_ICONINFORMATION);

    if (++m_currentLevel >= 5) {
        MessageBox(GetHWnd(), _T("恭喜你通关全部关卡！"), _T("游戏结束"), MB_OK);
        closegraph();
        exit(0);
    }
    initLevel();
}
