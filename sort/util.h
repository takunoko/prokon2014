#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3
#ifndef INCLUDED_UTIL_H
#define INCLUDED_UTIL_H
#include "pos.h"

extern int check_in_scope(int width, int height, int x, int y);
extern int check_pos_equal(int x1, int y1, int x2, int y2);
extern void surroundings(int *x, int *y, int direction);
extern void swap_pos(pos *p1, pos *p2);
extern int min(int num1, int num2);
extern int max(int num1, int num2);
extern void change_num(int *num1, int *num2);
extern int convert_num(int x, int y);
extern int convert_y(int num);
extern int convert_x(int num);

#endif
