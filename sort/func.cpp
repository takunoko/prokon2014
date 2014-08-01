#include <stdio.h>
#include "func.h"

// 範囲内にいるかチェック
int check_in_scope(int size_x, int size_y, int x, int y) {
  if(x >= 0 && x < size_x && y >= 0 && y < size_y)
    return 1;
  else return 0;
}

void disp_level_line(int num) {
  int i;

  printf("+");
  for(i = 0; i < num; i++) printf("-");
  printf("+");
  puts("");
}

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

void change_num(int *num1, int *num2) {
  int dummy = *num1;
  *num1 = *num2;
  *num2 = dummy;
}

int convert_num(int x, int y) {
  return x + y * 0x10;
}

// 数字を列の数字に変換
int convert_y(int num) {
  return (num / 0x10);
}

// 数字を行の数字に変換
int convert_x(int num) {
  return (num % 0x10);
}
