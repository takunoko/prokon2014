#include <stdio.h>
#include <stdlib.h>
#include "Pos.h"
#include "util.h"
#include "HIData.h"
#define PRINT 1

HIData::HIData() {
}

HIData::HIData(int w, int h) {
  width = w;
  height = h;
  md = 0;

  data = new int[w*h];
  distance = new int[w*h];
}

void HIData::calcMD() {
  int i, j;

  md = 0;
  for(i = 0; i < height; i++) {
    for(j = 0; j < width; j++) {
      // 選択しているところのMDは加算しないほうがいいかも
      if(!checkPosEqual(j, i, selected.x, selected.y))
        md += abs(data[convertHex(j, i)]%width-j) + abs(data[convertHex(j, i)]/width-i);
    }
  }
}

void HIData::dispData() {
  int i, j;

  for(i = 0; i < height; i++) {
    for(j = 0; j < width; j++) {
      printf("%3d", data[convertHex(j, i)]);
    }
    puts("");
  }
  puts("");
}

void HIData::findAndSelectData(int p) {
  int fd;

  fd = findData(p);
  selectData(fd);
}

int HIData::findData(int p) {
  int i;

  if(p < 0 || p >= width*height) myerror(1);
  for(i = 0; i < this->height; i++) {
    if(i == p) return i;
  }
#if PRINT
  puts("Cant find data");
#endif
  return -1;
}

int HIData::getChangedNum() {
  return changed_num;
}

int HIData::getLastMove() {
  return process.back();
}

int HIData::getMD() {
  return md;
}

Pos HIData::getSelected() {
  return selected;
}

string HIData::getStringSortData() {
  string str = "";
  char ss[3];
  int i, j;
  list<int>::iterator p1_x, p1_y, p2, p3;

  p1_x = selected_x.begin();
  p1_y = selected_y.begin();
  p2 = changed_nums.begin();
  p3 = process.begin();
  str += to_string(selected_num);
  str += "\r\n";
  for(i = 0; i < selected_num; i++, p1_x++, p1_y++, p2++) {
    sprintf(ss, "%X", *p1_x);
    str += ss;
    sprintf(ss, "%X", *p1_y);
    str += ss;
    str += "\r\n";
    str += to_string(*p2);
    str += "\r\n";
    for(j = 0; j < *p2; j++, p3++) {
      str += getDirectionChar(*p3);
    }
    str += "\r\n";
  }
  return str;
}

void HIData::importData(PosData &import_data) {
  int i, j;

  //if(checkData(import_data)) myerroR(1);
  for(i = 0; i < import_data.getHeight(); i++) {
    for(j = 0; j < import_data.getWidth(); j++) {
      data[convertHex(j, i)] = convertHex(import_data.getX(j, i), import_data.getY(j, i));
    }
  }
  dispData();
}

void HIData::selectData(int p) {
  if(p < 0 || p >= width*height) myerror(1);
  if(p == convertHex(selected.x, selected.y)) return;
  selected = Pos(convertX(p), convertY(p));
  move_flag = 0;
}

void HIData::swapData(int p1, int p2) {
  swapNum(&data[p1], &data[p2]);
}

int HIData::swapSelected(int pos) {
  if(direction == EQUAL) {
#if PRINT
    puts("direction = EQUAL");
#endif
    return 1;
  } else if(direction % 2 == 1) {
#if PRINT
    puts("naname direction");
#endif
    return 1;
  }
  if(move_flag == 0) {
    selected_num++;

    selected_x.push_back(selected.x);
    selected_y.push_back(selected.y);
    changed_nums.push_back(0);
    move_flag = 1;
  }
  Pos p = surroundings(selected, direction);
  if(!checkInScope(width, height, p.x, p.y)) return 1;
  swapData(, selected.x, selected.y);
  selected = surroundings(selected, direction);
  changed_num++;
  (changed_nums.back())++;
  process.push_back(direction);
  calcMD();

  return 0;
}

void HIData::undo() {
  if((changed_nums.back()) <= 0) {
    changed_nums.pop_back();
    selected_x.pop_back();
    selected_y.pop_back();
    selected_num--;
  }
  //1if(changed_nums.back() <= 0) return;
  int direction = getReversedDirection((process.back()));
  //process.pop_back();
  Pos p = surroundings(selected, direction);
  swapData(p.x, p.y, selected.x, selected.y);
  selected = surroundings(selected, direction);
  process.pop_back();
  (changed_nums.back())--;
  changed_num--;
  calcMD();
  //calcMD();
}
