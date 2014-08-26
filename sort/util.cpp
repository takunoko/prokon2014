#include <stdio.h>
#include <stdlib.h>
#include "util.h"

int checkPosEqual(int x1, int y1, int x2, int y2) {
  return ((x1 == x2) && (y1 == y2));
}

int checkPosEqual(Pos p1, Pos p2) {
  return checkPosEqual(p1.x, p1.y, p2.x, p2.y);
}

int getDirection(Pos source, Pos destination) {
  if(source.x == destination.x && source.y == destination.y) {
    return EQUAL;
  } else if(source.x == destination.x && source.y > destination.y) {
    return UP;
  } else if(source.x < destination.x && source.y > destination.y) {
    return UPPERRIGHT;
  } else if(source.x < destination.x && source.y == destination.y) {
    return RIGHT;
  } else if(source.x < destination.x && source.y < destination.y) {
    return LOWERRIGHT;
  } else if(source.x == destination.x && source.y < destination.y) {
    return DOWN;
  } else if(source.x > destination.x && source.y < destination.y) {
    return LOWERLEFT;
  } else if(source.x > destination.x && source.y == destination.y) {
    return LEFT;
  } else if(source.x > destination.x && source.y > destination.y) {
    return UPPERLEFT;
  }
  return EQUAL;
}

int getReversedDirection(int direction) {
  return (direction + DIRECTION_NUM / 2) % DIRECTION_NUM;
}

int getDirectionLR(int ox, int x) {
  if(ox == x) return EQUAL;
  return (ox < x) ? RIGHT : LEFT;
}

int getDirectionUD(int oy, int y) {
  if(oy == y) return EQUAL;
  return (oy < y) ? DOWN : UP;
}

int isNext(Pos p1, Pos p2) {
  return ((p1.x+1 == p2.x || p1.x-1 == p2.x || p1.x == p2.x) && (p1.y-1 == p2.y || p1.y+1 == p2.y || p1.y == p2.y));
}

int isNextX(Pos p1, Pos p2) {
  return (p1.x+1 == p2.x || p1.x-1 == p2.x || p1.x == p2.x);
}

int isNextY(Pos p1, Pos p2) {
  return (p1.y-1 == p2.y || p1.y+1 == p2.y || p1.y == p2.y);
}

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
  case UPPERRIGHT:
    (*x)++;
    (*y)--;
    break;
  case LOWERRIGHT:
    (*x)++;
    (*y)++;
    break;
  case LOWERLEFT:
    (*x)--;
    (*y)++;
    break;
  case UPPERLEFT:
    (*x)--;
    (*y)--;
    break;
  }
}

Pos surroundings(Pos Data, int direction) {
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
  case UPPERRIGHT:
    Data.x++;
    Data.y--;
    break;
  case LOWERRIGHT:
    Data.x++;
    Data.y++;
    break;
  case LOWERLEFT:
    Data.x--;
    Data.y++;
    break;
  case UPPERLEFT:
    Data.x--;
    Data.y--;
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
