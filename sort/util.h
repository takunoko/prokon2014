#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

#ifndef INCLUDED_UTIL_H
#define INCLUDED_UTIL_H
#include "Pos.h"

extern void swapNum(int *num1, int *num2);
extern int  checkInScope(int width, int height, int x, int y);
extern int  checkPosEqual(int x1, int y1, int x2, int y2);
extern int  convertNum(int x, int y);
extern int  convertY(int num);
extern int  convertX(int num);
extern int  max(int num1, int num2);
extern int  min(int num1, int num2);
extern void surroundings(int *x, int *y, int direction);
extern void swapPos(Pos *p1, Pos *p2);

#endif
