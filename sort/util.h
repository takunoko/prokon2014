#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3
#define checkInScope(width, height, x, y) (x>=0 && x<width && y >=0 && y<height)
#define checkPosEqual(x1, y1, x2, y2) (x1 == x2 && y1 == y2)
#define convertHex(x, y) x+y*0x10
#define convertX(num) num%0x10
#define convertY(num) num/0x10
#define max(num1, num2) num1>num2?num1:num2
#define min(num1, num2) num1<num2?num1:num2

#ifndef INCLUDED_UTIL_H
#define INCLUDED_UTIL_H
#include "Pos.h"

extern void swapNum(int *num1, int *num2);
extern void surroundings(int *x, int *y, int direction);
extern void swapPos(Pos *p1, Pos *p2);
extern void myerror(int error_code);

#endif
