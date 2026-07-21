#pragma once
// 坐标类：封装迷宫中的行列位置，x 为列（水平），y 为行（垂直）

class Point {
public:
    int x, y;
    Point(int x = 0, int y = 0);
    bool operator==(const Point& other) const;  // 坐标相等判断
};
