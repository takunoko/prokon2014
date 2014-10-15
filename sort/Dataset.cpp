#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include "Dataset.h"
#include "PosData.h"
#include "util.h"
#define PRINT 1

using namespace std;

Dataset::Dataset() {
  height = width = selected_num = changed_num = move_flag = 0;
  selected.setZero();
}

Dataset::Dataset(int w, int h) {
  if(!checkInScope(MAXWIDTH, MAXHEIGHT, w, h)) myerror(1);
  this->width = w;
  this->height = h;
  this->selected_num = this->changed_num = this->move_flag = 0;
  makeArray();
}

Dataset::~Dataset() {
  deleteArray();
}

int Dataset::checkSorted() {
  int i, j;

  for(i = 0; i < height; i++)
    for(j = 0; j < width; j++)
      if(!checkPosEqual(data[i][j].x, data[i][j].y, j, i)) return 0;
  return 1;
}

int Dataset::checkData(PosData check_data) {
  int i, j;
  if(!checkPosEqual(check_data.getWidth(), check_data.getHeight(), width, height)) myerror(1);
  Pos *dummy = new Pos[width * height];

  for(i = 0; i < height; i++) {
    for(j = 0; j < width; j++) {
      if(!checkInScope(width, height, check_data.getX(j, i), check_data.getY(j, i))) {
        delete [] dummy;
        return 1;
      }
      dummy[height*check_data.getY(j, i) + check_data.getX(j, i)].x = check_data.getX(j, i);
      dummy[height*check_data.getY(j, i) + check_data.getX(j, i)].y = check_data.getY(j, i);
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
#if PRINT
  printf("(selected=%d, changed=%d)\n", selected_num, changed_num);
#endif
}

// xとyに負の値を入れると[]を付けない
void Dataset::dispData(int x, int y) {
#if PRINT
  int i, j;

  for(i = 0; i < this->height; i++) {
    for(j = 0; j < this->width; j++) {
      if(checkPosEqual(j, i, selected.x, selected.y))
        changeWordColor(GREEN);
      else if(checkPosEqual(j, i, x, y)) changeWordColor(BLUE);
      printf("%X%X ", this->data[i][j].x, this->data[i][j].y);
      defaultWordColor();
    }
    puts("");
  }
  puts("");
#endif
}

// distanceを表示
void Dataset::dispDistance() {
#if PRINT
  int i, j;

  for(i = 0; i < this->height; i++) {
    for(j = 0; j < this->width; j++) {
      printf("[%2d,%2d]", this->distance[i][j].x, this->distance[i][j].y);
    }
    puts("");
  }
  puts("");
#endif
}

void Dataset::findAndSelectData(int x, int y) {
  Pos fd;

  fd = findData(x, y);
  selectData(fd.x, fd.y);
}

void Dataset::findAndSelectData(Pos data) {
  findAndSelectData(data.x, data.y);
}

// data_xとdata_yの値の入ったdataの要素の場所をxとyに入れる
Pos Dataset::findData(int data_x, int data_y) {
  int i, j;
  Pos loc;

  if(!checkInScope(width, height, data_x, data_y)) myerror(1);
  for(i = 0; i < this->height; i++) {
    for(j = 0; j < this->width; j++) {
      if(checkPosEqual(this->data[i][j].x, this->data[i][j].y, data_x, data_y)) {
        loc.x = j;
        loc.y = i;
        return loc;
      }
    }
  }
#if PRINT
  puts("Cant find data");
#endif
  return Pos(-1, -1);
}

Pos Dataset::findData(Pos data) {
  return findData(data.x, data.y);
}

Pos Dataset::getData(int x, int y) {
  Pos dummy;
  if(!checkInScope(width, height, x, y)) myerror(1);
  dummy.x = data[y][x].x;
  dummy.y = data[y][x].y;

  return dummy;
}

Pos Dataset::getDistance(int x, int y) {
  Pos dummy;
  if(!checkInScope(width, height, x, y)) myerror(1);
  dummy.x = distance[y][x].x;
  dummy.y = distance[y][x].y;

  return dummy;
}

int Dataset::getWidth() {
  return this->width;
}

int Dataset::getHeight() {
  return this->height;
}

Pos Dataset::getSelected() {
  return selected;
}

Pos Dataset::getSelectedData() {
  return getData(selected.x, selected.y);
}

Pos Dataset::getSelectedDistance() {
  return getDistance(selected.x, selected.y);
}

string Dataset::getStringSortData() {
  string str = "";
  char ss[3];
  int i, j;
  list<int>::iterator p1_x, p1_y, p2, p3;

  p1_x = selected_x.begin();
  p1_y = selected_y.begin();
  p2 = changed_nums.begin();
  p3 = process.begin();
  str += to_string(selected_num);
  str += "\n";
  for(i = 0; i < selected_num; i++, p1_x++, p1_y++, p2++) {
    sprintf(ss, "%X", *p1_x);
    str += ss;
    sprintf(ss, "%X", *p1_y);
    str += ss;
    str += "\n";
    str += to_string(*p2);
    str += "\n";
    for(j = 0; j < *p2; j++, p3++) {
      str += getDirectionChar(*p3);
    }
    str += "\n";
  }
  return str;
}

int Dataset::getX(int ox, int oy) {
  if(!checkInScope(width, height, ox, oy)) myerror(1);
  return data[oy][ox].x;
}

int Dataset::getY(int ox, int oy) {
  if(!checkInScope(width, height, ox, oy)) myerror(1);
  return data[oy][ox].y;
}


// 未完成。コピーコンストラクタとか
void Dataset::importData(PosData &import_data) {
  int i, j;

  //if(checkData(import_data)) myerroR(1);
  for(i = 0; i < import_data.getHeight(); i++) {
    for(j = 0; j < import_data.getWidth(); j++) {
      this->data[i][j].x = import_data.getX(j, i);
      this->data[i][j].y = import_data.getY(j, i);
    }
  }
  dispData();
}

// dataとdistanceの領域を確保
void Dataset::makeArray() {
  int i, j;

  data = new Pos*[height];
  distance = new Pos*[height];
  for(i = 0; i < height; i++) {
    data[i] = new Pos[width];
    distance[i] = new Pos[width];
    for(j = 0; j < width; j++) {
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
  if(checkPosEqual(x, y, selected.x, selected.y)) return;
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
/*
void Dataset::swapDataSelected(int x, int y) {
  swapData(selected.x, selected.y, x, y);
  selected.x = x;
  selected.y = y;
  changed_num++;
  if(move_flag == 0) {
    selected_num++;
    move_flag = 1;
  }
}

void Dataset::swapDataSelected(Pos pos) {
  swapData(selected.x, selected.y, pos.x, pos.y);
  selected.x = pos.x;
  selected.y = pos.y;
  changed_num++;
  if(move_flag == 0) {
    selected_num++;
    move_flag = 1;
  }
}*/

void Dataset::swapNext(int x, int y, int direction) {
  int dx = x;
  int dy = y;

  if(!checkInScope(width, height, x, y)) myerror(1);
  if(direction < EQUAL || direction >= DIRECTION_NUM) myerror(1);
  surroundings(&dx, &dy, direction);
  swapData(dx, dy, x, y);
}

void Dataset::swapSelected(int direction) {
  if(direction == EQUAL) {
#if PRINT
    puts("direction = EQUAL");
#endif
    return;
  } else if(direction % 2 == 1) {
#if PRINT
    puts("naname direction");
#endif
    return;
  }
  if(move_flag == 0) {
    selected_num++;
    selected_x.push_back(selected.x);
    selected_y.push_back(selected.y);
    changed_nums.push_back(0);
    move_flag = 1;
  }
  swapNext(selected.x, selected.y, direction);
  surroundings(&selected.x, &selected.y, direction);
  changed_num++;
  (*--changed_nums.end())++;
  process.push_back(direction);
}
