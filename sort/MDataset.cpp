#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include "MDataset.h"
#include "PosData.h"
#include "util.h"
#define PRINT 1

using namespace std;

MDataset::MDataset() {
  height = width = selected_num = changed_num = move_flag = 0;
  selected.setZero();
}

MDataset::MDataset(int w, int h) {
  if(!checkInScope(MAXWIDTH, MAXHEIGHT, w, h)) myerror(1);
  this->width = w;
  this->height = h;
  this->selected_num = this->changed_num = this->move_flag = 0;
  makeArray();
}

MDataset::~MDataset() {
  deleteArray();
}

void MDataset::calcMD() {
  int i, j;

  mdistance = 0;
  for(i = 0; i < height; i++) {
    for(j = 0; j < width; j++) {
      // 選択しているところのMDは加算しないほうがいいかも
      if(!checkPosEqual(j, i, selected.x, selected.y))
        mdistance += abs(data[i][j].x-j) + abs(data[i][j].y-i);
    }
  }
}

int MDataset::checkSorted() {
  int i, j;

  for(i = 0; i < height; i++)
    for(j = 0; j < width; j++)
      if(!checkPosEqual(data[i][j].x, data[i][j].y, j, i)) return 0;
  return 1;
}

void MDataset::deleteArray() {
  int i;

  for(i = 0; i < this->height; i++) {
    delete [] data[i];
  }
  delete [] data;
}

void MDataset::dispCost() {
#if PRINT
  printf("(selected=%d, changed=%d)\n", selected_num, changed_num);
#endif
}

// xとyに負の値を入れると[]を付けない
void MDataset::dispData(int x, int y) {
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

void MDataset::findAndSelectData(int x, int y) {
  Pos fd;

  fd = findData(x, y);
  selectData(fd.x, fd.y);
}

void MDataset::findAndSelectData(Pos data) {
  findAndSelectData(data.x, data.y);
}

// data_xとdata_yの値の入ったdataの要素の場所をxとyに入れる
Pos MDataset::findData(int data_x, int data_y) {
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

Pos MDataset::findData(Pos data) {
  int i, j;
  Pos loc;

  if(!checkInScope(width, height, data.x, data.y)) myerror(1);
  for(i = 0; i < this->height; i++) {
    for(j = 0; j < this->width; j++) {
      if(checkPosEqual(this->data[i][j].x, this->data[i][j].y, data.x, data.y)) {
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

int MDataset::getChangedNum() {
  return changed_num;
}

Pos MDataset::getData(int x, int y) {
  Pos dummy;
  if(!checkInScope(width, height, x, y)) myerror(1);
  dummy.x = data[y][x].x;
  dummy.y = data[y][x].y;

  return dummy;
}

int MDataset::getDistance() {
  return mdistance;
}

int MDataset::getLastMove() {
  return process.back();
}

int MDataset::getWidth() {
  return this->width;
}

int MDataset::getHeight() {
  return this->height;
}

Pos MDataset::getSelected() {
  return selected;
}

Pos MDataset::getSelectedData() {
  return getData(selected.x, selected.y);
}

string MDataset::getStringSortData() {
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
    printf("-----%X%X\n", *p1_x, *p1_y);
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

int MDataset::getX(int ox, int oy) {
  if(!checkInScope(width, height, ox, oy)) myerror(1);
  return data[oy][ox].x;
}

int MDataset::getY(int ox, int oy) {
  if(!checkInScope(width, height, ox, oy)) myerror(1);
  return data[oy][ox].y;
}


// 未完成。コピーコンストラクタとか
void MDataset::importData(PosData &import_data) {
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
void MDataset::makeArray() {
  int i, j;

  data = new Pos*[height];
  for(i = 0; i < height; i++) {
    data[i] = new Pos[width];
    for(j = 0; j < width; j++) {
      data[i][j].x = j;
      data[i][j].y = i;
    }
  }
}

// dataをごちゃまぜにかき回す
void MDataset::randomizeData() {
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
void MDataset::resetData() {
  int i, j;

  for(i = 0; i < this->height; i++) {
    for(j = 0; j < this->width; j++) {
      this->data[i][j].x = j;
      this->data[i][j].y = i;
    }
  }
  selected_num = changed_num = move_flag = 0;
}

void MDataset::selectData(int x, int y) {
  if(!checkInScope(width, height, x, y)) myerror(1);
  if(checkPosEqual(x, y, selected.x, selected.y)) return;
  selected.x = x;
  selected.y = y;
  move_flag = 0;
}

void MDataset::swapData(int x1, int y1, int x2, int y2) {
  if(!checkInScope(width, height, x1, y1)) myerror(1);
  if(!checkInScope(width, height, x2, y2)) myerror(1);

  swapPos(&data[y1][x1], &data[y2][x2]);
}

void MDataset::swapNext(int x, int y, int direction) {
  int dx = x;
  int dy = y;

  if(!checkInScope(width, height, x, y)) return;
  if(direction < EQUAL || direction >= DIRECTION_NUM) myerror(1);
  surroundings(&dx, &dy, direction);
  swapData(dx, dy, x, y);
}

int MDataset::swapSelected(int direction) {
  if(direction == EQUAL) {
#if PRINT
    puts("direction = EQUAL");
#endif
    myerror(1);
  } else if(direction % 2 == 1) {
#if PRINT
    puts("naname direction");
#endif
    myerror(1);
  }
  if(move_flag == 0) {
    selected_num++;
    selected_x.push_back(selected.x);
    selected_y.push_back(selected.y);
    changed_nums.push_back(0);
    move_flag = 1;
  }

  Pos d = surroundings(selected, direction);
  if(!checkInScope(width, height, d.x, d.y)) return 1;
  
  if(direction == RIGHT || direction == LEFT) {
    /*if(getDirectionLR(data[selected.y][selected.x].x, selected.x) != direction) {
      mdistance--;
    } else {
      mdistance++;
    }*/
    if(getDirectionLR(data[d.y][d.x].x, d.x) == direction) {
      mdistance--;
    } else {
      mdistance++;
    }
  } else if(direction == UP || direction == DOWN){
    /*if(getDirectionUD(data[selected.y][selected.x].y, selected.y) != direction) {
      mdistance--;
    } else {
      mdistance++;
    }*/
    if(getDirectionUD(data[d.y][d.x].y, d.y) == direction) {
      mdistance--;
    } else {
      mdistance++;
    }
  }
  swapNext(selected.x, selected.y, direction);
  surroundings(&selected.x, &selected.y, direction);
  changed_num++;
  (changed_nums.back())++;
  process.push_back(direction);

  return 0;
}

void MDataset::undo() {
  //if(changed_nums.back() <= 0) return;
  swapSelected(getReversedDirection((process.back())));
  //process.pop_back();
  process.pop_back();
  (changed_nums.back())-=1;
  changed_num-=1;
}
