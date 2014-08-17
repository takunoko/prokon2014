#ifndef INCLUDED_UTIL_H
#define INCLUDED_UTIL_H
#include "Pos.h"
#include <stdio.h>

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3
#define BLACK 0
#define RED 1
#define GREEN 2
#define YELLOW 3
#define BLUE 4
#define MAGENTA 5
#define CYAN 6
#define WHITE 7

#define changeWordColor(cc) printf("\033[3%dm", cc)
#define changeBackColor(cc) printf("\033[4%dm", cc)
#define checkInScope(width, height, x, y) (x>=0 && x<width && y >=0 && y<height)
#define checkPosEqual(x1, y1, x2, y2) (x1 == x2 && y1 == y2)
#define convertHex(x, y) x+y*0x10
#define convertX(num) num%0x10
#define convertY(num) num/0x10
#define defaultWordColor() printf("\033[39m")
#define defaultBackColor() printf("\033[49m")
#define getDirectionLR(ox, x) ((ox < x) ? RIGHT : LEFT)  // o?から見た方向
#define getDirectionUD(oy, y) ((oy < y) ? UP : DOWN)
#define max(num1, num2) num1>num2?num1:num2
#define min(num1, num2) num1<num2?num1:num2

extern void myerror(int error_code);
extern void swapNum(int *num1, int *num2); // num1とnum2を交換
extern void surroundings(int *x, int *y, int direction);
extern void swapPos(Pos *p1, Pos *p2);

#endif
