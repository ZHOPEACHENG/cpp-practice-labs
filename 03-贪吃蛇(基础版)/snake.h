#ifndef SNKAE_H
#define SNAKE_H

const int Hight = 20;
const int Width = 50;

extern int newMoveDirection;
extern int oldMoveDirection;
extern int food_x, food_y;
extern int snakeMatrix[Hight][Width];
extern int oldTail_x, oldTail_y;
extern int oldHead_x, oldHead_y;





void initSnake();
void drawPicture();
//void pcOperate();
//void operateSnake();
void moveSnakeByDirection();
void operateSnakeByDirection();
void zeroXy(int x, int y);
bool isCover();
void createFood();
void HideCursor();





#endif
