#include "PosData.h"
#include "stdio.h"
#include "stdlib.h"
#include "util.h"

PosData::PosData() {
}

PosData::PosData(int w, int h) {
  int i;

  width = w;
  height = h;
  data = new Pos*[height];
  for(i = 0; i < height; i++) {
    data[i] = new Pos[width];
  }
}

PosData::~PosData() {
  int i;

  for(i = 0; i < height; i++) {
    delete [] data[i];
  }
  delete [] data;
}

void PosData::dispData() {
  int i, j;

  for(i = 0; i < height; i++) {
    for(j = 0; j < width; j++) {
      printf("%X%X ", data[i][j].x, data[i][j].y);
    }
    puts("");
  }
  puts("");
}

int PosData::getHeight() {
  return this->height;
}
int PosData::getWidth() {
  return this->width;
}

int PosData::getX(int ox, int oy) {
  if(!checkInScope(width, height, ox, oy)) myerror(1);
  return data[oy][ox].x;
}

int PosData::getY(int ox, int oy) {
  if(!checkInScope(width, height, ox, oy)) myerror(1);
  return data[oy][ox].y;
}

void PosData::randomizeData() {
  int i, j, x, y;

  for(i = 0; i < height; i++) {
    for(j = 0; j < width; j++) {
      x = rand() % width;
      y = rand() % height;
      swapPos(&data[i][j], &data[y][x]);
    }
  }
}

void PosData::setX(int ox, int oy, int x) {
  if(!checkInScope(width, height, ox, oy)) myerror(1);
  data[oy][ox].x = x;
}

void PosData::setY(int ox, int oy, int y) {
  if(!checkInScope(width, height, ox, oy)) myerror(1);
  data[oy][ox].y = y;
}

void PosData::setData(int ox, int oy, int x, int y) {
  data[oy][ox].x = x;
  data[oy][ox].y = y;
}

void PosData::swapData(int x1, int y1, int x2, int y2) {
  swapPos(&data[y1][x1], &data[y2][x2]);
}

void PosData::swapData(int p1, int p2) {
  int x1, x2, y1, y2;

  x1 = p1 % width;
  x2 = p2 % width;
  y1 = p1 / width;
  y2 = p2 / width;
  swapPos(&data[y1][x1], &data[y2][x2]);
}
