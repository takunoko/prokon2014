#include <stdio.h>
#include <stdlib.h>
#include "Pos.h"
#include "util.h"
#include "id_df_search.h"
#define PRINT 1
#define DEBUG 0

ID_Data::ID_Data() : PosData() {
}

ID_Data::ID_Data(int w, int h) : PosData(w, h) {
  md = 0;
  int i, j;

  distance = new Pos*[height];
  for(i = 0; i < height; i++) {
    distance[i] = new Pos[width];
    for(j = 0; j < width; j++) {
      distance[i][j].x = 0;
      distance[i][j].y = 0;
    }
  }
}

void ID_Data::calcMD() {
  int i, j;

  md = 0;
  for(i = 0; i < height; i++) {
    for(j = 0; j < width; j++) {
      // 選択しているところのMDは加算しないほうがいいかも
      if(!checkPosEqual(j, i, selected.x, selected.y))
        md += abs(data[i][j].x-j) + abs(data[i][j].y-i);
    }
  }
}

void ID_Data::findAndSelectData(Pos p) {
  Pos fd;

  fd = findData(p);
  selectData(fd);
}

// data_xとdata_yの値の入ったdataの要素の場所をxとyに入れる
Pos ID_Data::findData(Pos p) {
  int i, j;
  Pos loc;

#if DEBUG
  if(!checkInScope(width, height, p.x, p.y)) myerror(1);
#endif
  for(i = 0; i < this->height; i++) {
    for(j = 0; j < this->width; j++) {
      if(checkPosEqual(this->data[i][j], p)) {
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

int ID_Data::getChangedNum() {
  return changed_num;
}

int ID_Data::getLastMove() {
  return process.back();
}

int ID_Data::getMD() {
  return md;
}

Pos ID_Data::getSelected() {
  return selected;
}

string ID_Data::getStringSortData() {
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

void ID_Data::importData(PosData &import_data) {
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

void ID_Data::selectData(Pos p) {
#if DEBUG
  if(!checkInScope(width, height, p.x, p.y)) myerror(1);
#endif
  if(checkPosEqual(p.x, p.y, selected.x, selected.y)) return;
  selected = p;
  move_flag = 0;
}

void ID_Data::setDistance(int x, int y) {
#if DEBUG
  if(!checkInScope(width, height, x, y)) myerror(1);
#endif
  distance[y][x].x = abs(data[y][x].x - x);
  distance[y][x].y = abs(data[y][x].y - y);
}

void ID_Data::swapData(int x1, int y1, int x2, int y2) {
  swapPos(&data[y1][x1], &data[y2][x2]);
  setDistance(x1, y1);
  setDistance(x2, y2);
}

int ID_Data::swapSelected(int direction) {
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
#if DEBUG
  if(!checkInScope(width, height, p.x, p.y)) return 1;
#endif
  //swapPos(&data[p.y][p.y], &data[selected.y][selected.x]);
  swapData(p.x, p.y, selected.x, selected.y);
  /*
  if(direction == UP || direction == DOWN) {
    int dummy = distance[p.y][p.x].y;
    distance[p.y][p.x].y = abs(data[p.y][p.x].y - p.y);
    distance[selected.y][selected.x].y = abs(data[selected.y][selected.x].y - selected.y);
    (dummy > distance[selected.y][selected.y].y) ? md-- : md++;
  }
  if(direction == RIGHT || direction == LEFT) {
    int dummy = distance[p.y][p.x].x;
    distance[p.y][p.x].x = abs(data[p.y][p.x].x - p.x);
    distance[selected.y][selected.x].x = abs(data[selected.y][selected.x].x - selected.x);
    (dummy > distance[selected.y][selected.y].x) ? md-- : md++;
  }*/
  selected = surroundings(selected, direction);
  changed_num++;
  (changed_nums.back())++;
  process.push_back(direction);
  calcMD();

  return 0;
}

void ID_Data::undo() {
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
