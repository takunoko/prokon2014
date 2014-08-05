#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3
#ifndef INCLUDED_FUNC_H
#define INCLUDED_FUNC_H

extern int check_in_scope(int size_x, int size_y, int x, int y);
extern void disp_level_line(int num);
extern void surroundings(int *x, int *y, int direction);
extern int min(int num1, int num2);
extern int max(int num1, int num2);
extern void change_num(int *num1, int *num2);
extern int convert_num(int x, int y);
extern int convert_y(int num);
extern int convert_x(int num);

#endif
