#include "PosData.h"
#include "stdio.h"

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

void PosData::setX(int ox, int oy, int x) {
  data[oy][ox].x = x;
}

void PosData::setY(int ox, int oy, int y) {
  data[oy][ox].y = y;
}

void PosData::setData(int ox, int oy, int x, int y) {
  data[oy][ox].x = x;
  data[oy][ox].y = y;
}
