#include <stdio.h>
#include <stdlib.h>
#include "Process3.h"
#include "util.h"

using namespace std;

Pro3::Process3(): ProcessBase() {
  target.setZero();
  target_data.setZero();
}

Pro3::Process3(int w, int h): ProcessBase(w, h) {
  target.setZero();
  target_data.setZero();
}

void Pro3::dispSorted() {
  list<Pos>::iterator p;

  for(p = sorted.begin(); p != sorted.end(); p++) {
    printf("%d, %d\n", p->x, p->y);
  }
  puts("");
}

// 安全
int Pro3::isSelectedNextToTarget() {
  Pos s = table->getSelected();

  return isNext(s, target);
  //return ((s.x+1 == target.x || s.x-1 == target.x || s.x == target.x) && (s.y-1 == target.y || s.y+1 == target.y || s.y == target.y));
}

void Pro3::moveSelectedNextTarget() {
  Pos s = table->getSelected();
  list<Pos>::iterator p;

  int directionLR;
  int directionUD;
  int move_flagLR;
  int move_flagUD;

  puts("-------moveSelectedNextTarget-------");
  while(!isSelectedNextToTarget()) {
    directionLR = getDirectionLR(s.x, target.x);
    directionUD = getDirectionUD(s.y, target.y);
    move_flagLR = 1;
    move_flagUD = 1;
    for(p = sorted.begin(); p != sorted.end(); p++) {
      // 隣に来たらでいいかも
      if(checkPosEqual(surroundings(s, directionUD), *p))
        move_flagUD = 0;
      if(checkPosEqual(surroundings(s, directionLR), *p))
        move_flagLR = 0;
    }
    if(move_flagUD && !isNextY(target, table->getSelected())) table->swapSelected(directionUD);
    if(move_flagLR && !isNextX(target, table->getSelected())) table->swapSelected(directionLR);
  }
  table->dispData(target.x, target.y);
}

int Pro3::moveTarget() {
  list<Pos>::iterator p;
  int directionUD = getDirectionUD(target.y, target_data.y);
  int directionLR = getDirectionLR(target.x, target_data.x);
  int movflag_x = 1;
  int movflag_y = 1;

  if(checkPosEqual(target, target_data)) return 1;
  // targetの隣へ
  moveSelectedNextTarget();
  // 動かすループ
  while(target.x != target_data.x) {
    directionUD = getDirectionUD(target.y, target_data.y);
    directionLR = getDirectionLR(target.x, target_data.x);
    movflag_x = 1;
    movflag_y = 1;
    for(p = sorted.begin(); p != sorted.end(); p++) {
      if(checkPosEqual(target, target_data)) return 1;
      // 横がソートされてたら縦をソートする
      // に変える
      if(checkPosEqual(surroundings(target, directionUD), *p)) {
        movflag_x = 0;
      }
    }
    
    if(movflag_x) {
      rotateSelected(directionLR);
      table->swapSelected(getReversedDirection(directionLR));
      table->dispData();
    } else {
      rotateSelected(directionUD);
      table->swapSelected(getReversedDirection(directionUD));
    }
    target = table->findData(target_data);
    if(checkPosEqual(target, target_data)) return 1;

    for(p = sorted.begin(); p != sorted.end(); p++) {
      // 横がソートされてたら縦をソートする
      // に変える
      if(checkPosEqual(surroundings(target, directionUD), *p)) {
        movflag_y = 0;
      }
    }

    if(movflag_y) {
      rotateSelected(directionUD);
      table->swapSelected(getReversedDirection(directionUD));
    } else {
      rotateSelected(directionLR);
      table->swapSelected(getReversedDirection(directionLR));
    }
    target = table->findData(target_data);
  }
  while(target.y != target_data.y) {
    //for(p = sorted.begin(); p != sorted.end(); p++) {
      directionUD = getDirectionUD(target.y, target_data.y);
      rotateSelected(directionUD);
      table->swapSelected(getReversedDirection(directionUD));
      target = table->findData(target_data);
    //}
  }
  return 0;
}

void Pro3::rotateSelected(int direction) {
  // int dir_val = (direction + DIRECTION_NUM / 2) % DIRECTION_NUM;
  // directionは動かす方向
  // targetから見たselectedの方向
  int dir_selected = getDirection(target, table->getSelected());
  int move_distance = abs(direction - dir_selected);
  // 1か-1
  // 間違ってる
  int move_direction = dir_selected > direction ? -1 : 1;

  if(checkPosEqual(table->getSelected(), surroundings(target, direction))) return;
  if(!checkInScope(table->getWidth(), table->getHeight(), surroundings(target, direction).x, surroundings(target, direction).y)) {
    puts("cant ratate...");
    return;
  }

  if(move_distance > DIRECTION_NUM / 2) {
    move_distance = DIRECTION_NUM - move_distance;
    move_direction *= -1;
  }

  Pos dummy_target = target;
  int i;
  int reverse_flag = 0;
  list<Pos>::iterator p;

  for(i = dir_selected; i != direction; i = (i + move_direction + DIRECTION_NUM) % DIRECTION_NUM) {
    if(!checkInScope(table->getWidth(), table->getHeight(), surroundings(dummy_target, i).x, surroundings(dummy_target, i).y)) reverse_flag = 1;
    for(p = sorted.begin(); p != sorted.end(); p++) {
      if(checkPosEqual(surroundings(dummy_target, i), *p)) {
        reverse_flag = 1;
        break;
      }
    }
    if(reverse_flag == 1) {
      move_direction *= -1;
      break;
    }
  }

  int move_dir;
  int j = dir_selected;
  puts("-------rotateSelected-------");
  printf("move_direction = %d\n", move_direction);
  printf("move_distance = %d\n", move_distance);
  for(i = 0; i < move_distance; i++) {
    // 目的地についたら止めたほうがいいかも
    j = (j + move_direction + DIRECTION_NUM) % DIRECTION_NUM;
    move_dir = getDirection(table->getSelected(), surroundings(target, j));
    printf("move_dir = %d\n", move_dir);
    table->swapSelected(move_dir);
    table->dispData(target.x, target.y);
  }
}

void Pro3::sort() {
  int i, j;

  table->dispData();
  // 一番右下になるデータを選択
  table->findAndSelectData(table->getWidth()-1, table->getHeight()-1);
  /*target = table->findData(0, 0);
  moveSelectedNextTarget();
  rotateSelected(UP);
  table->dispCost();

  return;
*/
  // 上半分
  for(i = 0; i < table->getHeight() - 2; i++) {
    // 端以外の
    for(j = 0; j < table->getWidth() - 1; j++) {
      target_data = Pos(j, i);
      target = table->findData(target_data);
      table->dispData(target.x, target.y);
      // 目的地へ移動
      moveTarget();
      target = table->findData(target_data);
      sorted.push_back(target);
    }
    // 一番右側
    // ソートした部分をずらす
    // 一番右側のを正しい位置に
    // ずらしたのを戻す
    // sorted.push_back(target);
  }
  table->dispData();

  // 下二列
}
