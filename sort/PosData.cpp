#include "PosData.h"
#include "stdio.h"
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
      printf("%X%X", data[i][j].x, data[i][j].y);
    }
    puts("");
  }
}

Pos **PosData::getData() {
  return data;
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
