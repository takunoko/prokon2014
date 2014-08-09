#include <stdio.h>
#include <stdlib.h>
#include "Dataset.h"
#include "util.h"

Dataset::Dataset() {
  height = width = selected_num = changed_num = move_flag = 0;
  selected.x = 0;
  selected.y = 0;
}

Dataset::Dataset(int w, int h) {
  if(w < 0 || h < 0) myerror(1);
  this->width = w;
  this->height = h;
  this->selected_num = this->changed_num = this->move_flag = 0;
  makeArray();
}

Dataset::~Dataset() {
  deleteArray();
}

int Dataset::checkData(Pos **check_data) {
  int i, j;
  Pos *dummy = new Pos[width * height];

  for(i = 0; i < height; i++) {
    for(j = 0; j < width; j++) {
      if(!checkInScope(width, height, check_data[i][j].x, check_data[i][j].y)) {
        delete [] dummy;
        return 1;
      }
      dummy[height*check_data[i][j].y + check_data[i][j].x].x = check_data[i][j].x;
      dummy[height*check_data[i][j].y + check_data[i][j].x].y = check_data[i][j].y;
    }
  }
  for(i = 0; i < height*width; i++) {
    if(dummy[i].x < 0 || dummy[i].y < 0) {
      delete [] dummy;
      return 1;
    }
  }
  delete [] dummy;
  return 0;
}

void Dataset::deleteArray() {
  int i;

  for(i = 0; i < this->height; i++) {
    delete [] data[i];
    delete [] distance[i];
  }
  delete [] data;
  delete [] distance;
}

void Dataset::dispCost() {
  printf("(selected=%d, changed=%d)\n", selected_num, changed_num);
}

// xとyに負の値を入れると[]を付けない
void Dataset::dispData(int x, int y) {
  int i, j;

  for(i = 0; i < this->width; i++) {
    for(j = 0; j < this->height; j++) {
      if(checkPosEqual(j, i, x, y) || checkPosEqual(j, i, selected.x, selected.y)) {
        changeWordColor(GREEN);
      }
      printf("%X%X ", this->data[i][j].x, this->data[i][j].y);
      defaultWordColor();
    }
    puts("");
  }
  puts("");
}

// distanceを表示
void Dataset::dispDistance() {
  int i, j;

  for(i = 0; i < this->width; i++) {
    for(j = 0; j < this->height; j++) {
      printf("[%2d,%2d]", this->distance[i][j].x, this->distance[i][j].y);
    }
    puts("");
  }
  puts("");
}

// data_xとdata_yの値の入ったdataの要素の場所をxとyに入れる
int Dataset::findData(int data_x, int data_y, int *x, int *y) {
  int i, j;

  if(!checkInScope(width, height, data_x, data_y)) myerror(1);
  for(i = 0; i < this->height; i++) {
    for(j = 0; j < this->width; j++) {
      if(checkPosEqual(this->data[i][j].x, this->data[i][j].y, data_x, data_y)) {
        *x = j;
        *y = i;
        return 0;
      }
    }
  }
  return 1;
}

int Dataset::getWidth() {
  return this->width;
}

int Dataset::getHeight() {
  return this->height;
}

// 未完成。コピーコンストラクタとか
void Dataset::importData(Pos **import_data) {
  if(checkData(import_data)) myerror(1);
  this->data = import_data;
}

// dataとdistanceの領域を確保
void Dataset::makeArray() {
  int i, j;

  data = new Pos*[height];
  distance = new Pos*[height];
  for(i = 0; i < this->height; i++) {
    data[i] = new Pos[width];
    distance[i] = new Pos[width];
    for(j = 0; j < this->width; j++) {
      data[i][j].x = j;
      data[i][j].y = i;
      distance[i][j].x = 0;
      distance[i][j].y = 0;
    }
  }
}

// dataをごちゃまぜにかき回す
void Dataset::randomizeData() {
  int i, j, x, y;

  resetData();

  for(i = 0; i < height; i++) {
    for(j = 0; j < width; j++) {
      x = rand() % width;
      y = rand() % height;
      swapData(j, i, x, y);
    }
  }
}

// dataを綺麗にソートされた状態にする
void Dataset::resetData() {
  int i, j;

  for(i = 0; i < this->height; i++) {
    for(j = 0; j < this->width; j++) {
      this->data[i][j].x = j;
      this->data[i][j].y = i;
      this->distance[i][j].x = 0;
      this->distance[i][j].y = 0;
    }
  }
  selected_num = changed_num = move_flag = 0;
}

void Dataset::selectData(int x, int y) {
  if(!checkInScope(width, height, x, y)) myerror(1);
  selected.x = x;
  selected.y = y;
  move_flag = 0;
}

void Dataset::setDistance(int x, int y) {
  if(!checkInScope(width, height, x, y)) myerror(1);
  distance[y][x].x = data[y][x].x - x;
  distance[y][x].y = data[y][x].y - y;
}

void Dataset::swapData(int x1, int y1, int x2, int y2) {
  if(!checkInScope(width, height, x1, y1)) myerror(1);
  if(!checkInScope(width, height, x2, y2)) myerror(1);

  swapPos(&data[y1][x1], &data[y2][x2]);
  setDistance(x1, y1);
  setDistance(x2, y2);
}

void Dataset::swapNext(int x, int y, int direction) {
  int dx = x;
  int dy = y;

  if(!checkInScope(width, height, x, y)) myerror(1);
  if(direction < 0 || direction > 3) myerror(1);
  surroundings(&dx, &dy, direction);
  swapData(dx, dy, x, y);
}

void Dataset::swapSelected(int direction) {
  swapNext(selected.x, selected.y, direction);
  surroundings(&selected.x, &selected.y, direction);
  changed_num++;
  if(move_flag == 0) {
    selected_num++;
    move_flag = 1;
  }
}
