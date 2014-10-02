#include <stdio.h>
#include <stdlib.h>
#include "Process4.h"
#include "util.h"

using namespace std;

Pro4::Process4(): ProcessBase() {
  target.setZero();
  target_data.setZero();
}

Pro4::Process4(int w, int h): ProcessBase(w, h) {
  target.setZero();
  target_data.setZero();
}

void Pro4::dispSorted() {
  list<Pos>::iterator p;

  for(p = sorted.begin(); p != sorted.end(); p++) {
    printf("%d, %d\n", p->x, p->y);
  }
  puts("");
}

// 安全
int Pro4::isSelectedNextToTarget() {
  Pos s = table->getSelected();

  return isNext(s, target);
}

void Pro4::moveSelectedNextTarget() {
  Pos s = table->getSelected();
  list<Pos>::iterator p;

  int directionLR;
  int directionUD;
  int move_flagLR;
  int move_flagUD;
  // ルート検索関数とか作ってここで使いたい
  // listにルートを保存しとく…？

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
    printf("%d, %d\n", move_flagLR, move_flagUD);
    table->dispData(target.x, target.y);
    // isNextYとか入れてると出来ない
    if(move_flagUD && !isNextY(target, table->getSelected())) table->swapSelected(directionUD);
    if(move_flagLR && !isNextX(target, table->getSelected())) table->swapSelected(directionLR);
  }
  table->dispData(target.x, target.y);
}

int Pro4::moveTarget(Pos pos) {
  list<Pos>::iterator p;
  int directionUD = getDirectionUD(target.y, pos.y);
  int directionLR = getDirectionLR(target.x, pos.x);
  int movflag_x = 1;
  int movflag_y = 1;

  if(checkPosEqual(target, pos)) return 1;
  // targetの隣へ
  moveSelectedNextTarget();
  // 動かすループ
  while(target.x != pos.x) {
    directionUD = getDirectionUD(target.y, pos.y);
    directionLR = getDirectionLR(target.x, pos.x);
    movflag_x = 1;
    movflag_y = 1;
    if(checkPosEqual(target, pos)) return 1;
    for(p = sorted.begin(); p != sorted.end(); p++) {
      // 横がソートされてたら縦をソートする
      // に変える
      if(checkPosEqual(surroundings(target, directionLR), *p)) {
        movflag_x = 0;
      }
    }
    
    if(movflag_x) {
      rotateSelected(directionLR);
      table->swapSelected(getReversedDirection(directionLR));
      table->dispData();
    } /*else {
      rotateSelected(directionUD);
      table->swapSelected(getReversedDirection(directionUD));
    }*/

    directionUD = getDirectionUD(target.y, pos.y);
    directionLR = getDirectionLR(target.x, pos.x);
    target = table->findData(pos);
    if(checkPosEqual(target, pos)) return 1;

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
    } /*else {
      rotateSelected(directionLR);
      table->swapSelected(getReversedDirection(directionLR));
    }*/
    target = table->findData(pos);
  }
  while(target.y != pos.y) {
    //for(p = sorted.begin(); p != sorted.end(); p++) {
      directionUD = getDirectionUD(target.y, pos.y);
      rotateSelected(directionUD);
      table->swapSelected(getReversedDirection(directionUD));
      target = table->findData(pos);
    //}
  }
  return 0;
}

void Pro4::rotateSelected(int direction) {
  // int dir_val = (direction + DIRECTION_NUM / 2) % DIRECTION_NUM;
  // directionは動かす方向
  // targetから見たselectedの方向
  int dir_selected = getDirection(target, table->getSelected());
  int move_distance = abs(direction - dir_selected);
  // 1か-1
  // 間違ってる
  //-----------------------------直すところ
  int move_direction = dir_selected > direction ? -1 : 1;

  if(direction == EQUAL) return;
  if(checkPosEqual(table->getSelected(), surroundings(target, direction)))  {
    puts("selected dont need to rotate");
    return;
  }
  if(!checkInScope(table->getWidth(), table->getHeight(), surroundings(target, direction).x, surroundings(target, direction).y)) {
    puts("cant rotate...");
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
      move_distance = DIRECTION_NUM - move_distance;
      move_direction *= -1;
      break;
    }
  }

  int move_dir;
  int j = dir_selected;
  puts("-------rotateSelected-------");
  printf("move_direction = %d\n", move_direction);
  printf("move_distance = %d\n", move_distance);
  printf("direction = %d\n", direction);
  printf("target = %d, %d\n", target.x, target.y);
  for(i = 0; i < move_distance; i++) {
    // 目的地についたら止めたほうがいいかも
    j = (j + move_direction + DIRECTION_NUM) % DIRECTION_NUM;
    move_dir = getDirection(table->getSelected(), surroundings(target, j));
    printf("j = %d\n", j);
    printf("move_dir = %d\n", move_dir);
    table->swapSelected(move_dir);
    table->dispData(target.x, target.y);
  }
  puts("end");
}

void Pro4::sort() {
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
    for(j = 0; j < table->getWidth() - 2; j++) {
      target_data = Pos(j, i);
      target = table->findData(target_data);
      table->dispData(target.x, target.y);
      // 目的地へ移動
      moveTarget(target_data);
      target = table->findData(target_data);
      sorted.push_back(target);
    }
    puts("alhsiudhfaisudhfoiauh");
    target_data = Pos(table->getWidth()-1, i);
    target = table->findData(target_data);
    moveTarget(Pos(table->getWidth()-2, i));
    target_data = Pos(table->getWidth()-2, i);
    target = table->findData(target_data);
    moveTarget(Pos(table->getWidth()-2, i+1));
    // 一番右側
    // ソートした部分をずらす
    // 一番右側のを正しい位置に
    // ずらしたのを戻す
    // sorted.push_back(target);
    return;
  }
  table->dispData();

  // 下二列
}
