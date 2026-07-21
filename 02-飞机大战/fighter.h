#ifndef FIGHTER_H
#define FIGHTER_H

const int height = 20;
const int width = 50;
const int MAX_ENEMY = 10;

extern int fighter_x, fighter_y;
extern int bullet_x, bullet_y;
extern int enemy_x[MAX_ENEMY], enemy_y[MAX_ENEMY];

extern int planeMatrix[height][width];
extern int speed;
extern int activeEnemyCount;
extern int spawnTimer;

//状态机
struct State {
	int scores;
	int hp;
	int level;
	bool initial = true;
	State(int s, int b, int l);
};

State init();
State drawPicture(State s);
State  pcOperate(State s);
State operateFighter(State s);
void zeroXy(int x, int y);
State enemyFighter(State s);
void floatBullet();
void HideCursor();
bool isCover(int y);
void resetEnemy(int index);
State isWin(State s);
State CollisionCheck(State s);
State setEnemys(State s);





#endif
