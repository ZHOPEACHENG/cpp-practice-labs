#include "clock.h"
#include <math.h>
#include <stdio.h>

// 背景
void drawBackground()
{
    setfillcolor(BG_COLOR);
    fillrectangle(0, 0, Width, High);
}

// 表盘
void drawClockDial(int cx, int cy, int r)
{
    // 外圈阴影
    setlinestyle(PS_SOLID, 6);
    setcolor(DIAL_SHADOW);
    circle(cx + 2, cy + 2, r + 8);

    // 外圈金属环
    setlinestyle(PS_SOLID, 5);
    setcolor(DIAL_OUTER1);
    circle(cx, cy, r + 5);
    setlinestyle(PS_SOLID, 2);
    setcolor(DIAL_OUTER2);
    circle(cx, cy, r + 2);

    // 表盘主面
    setfillcolor(DIAL_FACE);
    solidcircle(cx, cy, r);

    // 表盘边缘
    setlinestyle(PS_SOLID, 2);
    setcolor(DIAL_BORDER);
    circle(cx, cy, r);

    // 内圈装饰环
    setlinestyle(PS_SOLID, 1);
    setcolor(DIAL_DECOR);
    circle(cx, cy, r - 18);
}

// 刻度线
void drawTickMarks(int cx, int cy, int r)
{
    for (int i = 0; i < 60; i++)
    {
        // 从12点(PI/2)开始，顺时针递减
        double angle = PI / 2 - PI * 2 * i / 60;
        double outerR = r - 8;
        double innerR;
        int    x1 = cx + (int)(outerR * cos(angle));
        int    y1 = cy - (int)(outerR * sin(angle));

        if (i % 15 == 0)
        {
            innerR = r - 30;
            setlinestyle(PS_SOLID, 4);
            setcolor(TICK_HOUR);
            int x2 = cx + (int)(innerR * cos(angle));
            int y2 = cy - (int)(innerR * sin(angle));
            line(x1, y1, x2, y2);
        }
        else if (i % 5 == 0)
        {
            innerR = r - 22;
            setlinestyle(PS_SOLID, 2);
            setcolor(TICK_5MIN);
            int x2 = cx + (int)(innerR * cos(angle));
            int y2 = cy - (int)(innerR * sin(angle));
            line(x1, y1, x2, y2);
        }
        else
        {
            putpixel(x1, y1, TICK_MINUTE);
        }
    }
}

// 小时数字
void drawHourNumbers(int cx, int cy, int r)
{
    setbkmode(TRANSPARENT);
    int hourRadius = r - 52;

    for (int h = 1; h <= 12; h++)
    {
        // 12点=PI/2，顺时针每时减PI/6
        double angle = PI / 2 - h % 12 * PI / 6;
        int    hx = cx + (int)(hourRadius * cos(angle));
        int    hy = cy - (int)(hourRadius * sin(angle));

        TCHAR hourStr[4];
        _stprintf_s(hourStr, _T("%d"), h);

        int fontSize;
        if (h % 3 == 0)
        {
            fontSize = 26;
            setcolor(NUM_CARDINAL);
        }
        else
        {
            fontSize = 22;
            setcolor(NUM_NORMAL);
        }
        settextstyle(fontSize, 0, _T("Arial"));

        int textW = (h >= 10) ? fontSize : fontSize / 2;
        outtextxy(hx - textW / 2, hy - fontSize / 2, hourStr);
    }
}

// 中心帽
void drawCenterCap(int cx, int cy)
{
    setfillcolor(CAP_OUTER);
    setcolor(CAP_BORDER);
    solidcircle(cx, cy, 6);
}

// 指针
void drawHands(int cx, int cy, SYSTEMTIME ti)
{
    int secondLen = 130;
    int minuteLen = 110;
    int hourLen   = 75;

    // 原始角度（0=12点，顺时针递增）
    float sRaw = ti.wSecond * 2 * PI / 60;
    float mRaw = ti.wMinute * 2 * PI / 60 + sRaw / 60;
    float hRaw = ti.wHour % 12 * 2 * PI / 12 + mRaw / 12;

    float sAngle = PI / 2 - sRaw;
    float mAngle = PI / 2 - mRaw;
    float hAngle = PI / 2 - hRaw;

    // x = R·cosθ, y = -R·sinθ（y轴屏幕翻转）
    int sx = cx + (int)(secondLen * cos(sAngle));
    int sy = cy - (int)(secondLen * sin(sAngle));
    int mx = cx + (int)(minuteLen * cos(mAngle));
    int my = cy - (int)(minuteLen * sin(mAngle));
    int hx = cx + (int)(hourLen   * cos(hAngle));
    int hy = cy - (int)(hourLen   * sin(hAngle));

    // 时针，最粗
    setlinestyle(PS_SOLID, 8);
    setcolor(HAND_HOUR);
    line(cx, cy, hx, hy);

    // 分针
    setlinestyle(PS_SOLID, 5);
    setcolor(HAND_MINUTE);
    line(cx, cy, mx, my);

    // 秒针，最细
    setlinestyle(PS_SOLID, 1);
    setcolor(HAND_SECOND);
    line(cx, cy, sx, sy);

    // 秒针尾部（反方向）
    int tx = cx - (int)(25 * cos(sAngle));
    int ty = cy + (int)(25 * sin(sAngle));
    setcolor(HAND_SEC_TAIL);
    line(cx, cy, tx, ty);
}

// 当前时间
void drawCurrentTime(int cx, int y, SYSTEMTIME ti)
{
    settextstyle(16, 0, _T("Consolas"));
    TCHAR buf[30];
    _stprintf_s(buf, _T("%04d-%02d-%02d  %02d:%02d:%02d"),
        ti.wYear, ti.wMonth, ti.wDay,
        ti.wHour, ti.wMinute, ti.wSecond);
    setcolor(TIME_TEXT);
    int w = 175;
    outtextxy(cx - w / 2, y, buf);
}
