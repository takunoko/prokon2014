#include <stdio.h>
#include "util.h"

// 範囲内にいるかチェック
int checkInScope(int width, int height, int x, int y) {
  if(x >= 0 && x < width && y >= 0 && y < height)
    return 1;
  else return 0;
}

// x1, y1とx2, y2の値が等しいか調べる
int checkPosEqual(int x1, int y1, int x2, int y2) {
  return (x1 == x2 && y1 == y2);
}

// directionの方向にxとyを移動する
void surroundings(int *x, int *y, int direction) {
  switch(direction) {
  case UP: // 
    (*y)--;
    break;
  case RIGHT: //  
    (*x)++;
    break;
  case DOWN: //
    (*y)++;
    break;
  case LEFT: //  
    (*x)--;
    break;
  }
}

// p1とp2の値を交換する
void swapPos(Pos *p1, Pos *p2) {
  Pos dummy = *p1;
  *p1 = *p2;
  *p2 = dummy;
}

int min(int num1, int num2) {
  if(num1 <= num2) return num1;
  if(num2 < num1) return num2;
  return 0;
}

int max(int num1, int num2) {
  if(num1 >= num2) return num1;
  if(num2 > num1) return num2;
  return 0;
}

// num1とnum2の値を交換
void swapNum(int *num1, int *num2) {
  int dummy = *num1;
  *num1 = *num2;
  *num2 = dummy;
}

int convertNum(int x, int y) {
  return x + y * 0x10;
}

// 数字を列の数字に変換
int convertY(int num) {
  return (num / 0x10);
}

// 数字を行の数字に変換
int convertX(int num) {
  return (num % 0x10);
}
