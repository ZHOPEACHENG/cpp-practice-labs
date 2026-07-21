#pragma once
#include <windows.h>
#include "config.h"

void drawBackground();
void drawClockDial(int cx, int cy, int r);
void drawTickMarks(int cx, int cy, int r);
void drawHourNumbers(int cx, int cy, int r);
void drawCenterCap(int cx, int cy);
void drawHands(int cx, int cy, SYSTEMTIME ti);
void drawCurrentTime(int cx, int y, SYSTEMTIME ti);
