#include <stdio.h>
#include <stdlib.h>
#include "util.h"

void myerror(int error_code) {
  puts("MYERROR");
  exit(1);
}

// directionの方向にxとyを移動する
void surroundings(int *x, int *y, int direction) {
  switch(direction) {
  case UP:
    (*y)--;
    break;
  case RIGHT:
    (*x)++;
    break;
  case DOWN:
    (*y)++;
    break;
  case LEFT:
    (*x)--;
    break;
  }
}

Pos surroundingsp(Pos Data, int direction) {
  switch(direction) {
  case UP:
    Data.y--;
    break;
  case RIGHT:
    Data.x++;
    break;
  case DOWN:
    Data.y++;
    break;
  case LEFT:
    Data.x--;
    break;
  }
  return Data;
}

// num1とnum2の値を交換
void swapNum(int *num1, int *num2) {
  int dummy = *num1;
  *num1 = *num2;
  *num2 = dummy;
}

// p1とp2の値を交換する
void swapPos(Pos *p1, Pos *p2) {
  Pos dummy = *p1;
  *p1 = *p2;
  *p2 = dummy;
}
