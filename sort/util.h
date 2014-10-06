#ifndef INCLUDED_UTIL_H
#define INCLUDED_UTIL_H
#include "Pos.h"
#include <stdio.h>

#define EQUAL -1
#define UP 0
#define UPPERRIGHT 1
#define RIGHT 2
#define LOWERRIGHT 3
#define DOWN 4
#define LOWERLEFT 5
#define LEFT 6
#define UPPERLEFT 7
#define DIRECTION_NUM 8
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
#define convertHex(x, y) x+y*0x10
#define convertX(num) num%0x10
#define convertY(num) num/0x10
#define defaultWordColor() printf("\033[39m")
#define defaultBackColor() printf("\033[49m")

extern int checkPosEqual(int x1, int y1, int x2, int y2);
extern int checkPosEqual(Pos p1, Pos p2);
extern int getDirection(Pos source, Pos destination);
extern int getDirectionLR(int ox, int x);
extern int getDirectionUD(int oy, int y);
extern int getReversedDirection(int direction);
extern int isConnected(Pos p1, Pos p2);
extern int isNext(Pos p1, Pos p2);
extern int isNextX(Pos p1, Pos p2);
extern int isNextY(Pos p1, Pos p2);
extern void myerror(int error_code);
extern void swapNum(int *num1, int *num2); // num1とnum2を交換
extern void surroundings(int *x, int *y, int direction);
extern Pos surroundings(Pos Data, int direction);
extern void swapPos(Pos *p1, Pos *p2);

#endif
