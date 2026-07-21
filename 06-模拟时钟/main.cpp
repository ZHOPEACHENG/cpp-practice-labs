#include "clock.h"
#include <conio.h>
#include <stdio.h>

int main()
{
    // 初始化图形窗口
    initgraph(Width, High);
    setbkcolor(BG_COLOR);

    // 时钟居中
    int clock_cx = Width / 2;
    int clock_cy = High / 2;
    int clock_r  = 155;

    SYSTEMTIME ti;
    BeginBatchDraw();  // 开始批量绘图，防止闪烁

    while (1)
    {
        GetLocalTime(&ti);  // 获取当前系统时间

        // ESC 退出
        if (_kbhit() && _getch() == 27)
            break;

        cleardevice();  // 清空画面

        // 绘制背景
        drawBackground();
        // 绘制表盘
        drawClockDial(clock_cx, clock_cy, clock_r);
        // 绘制刻度线
        drawTickMarks(clock_cx, clock_cy, clock_r);
        // 绘制小时数字
        drawHourNumbers(clock_cx, clock_cy, clock_r);
        // 绘制中心帽
        drawCenterCap(clock_cx, clock_cy);
        // 绘制三根指针
        drawHands(clock_cx, clock_cy, ti);
        // 绘制当前日期时间
        drawCurrentTime(clock_cx, clock_cy + clock_r + 25, ti);

        FlushBatchDraw();  // 刷新画面
        Sleep(1000);       // 每秒刷新
    }

    EndBatchDraw();
    closegraph();
    return 0;
}
