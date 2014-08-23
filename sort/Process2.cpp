#include "Process2.h"
#include <stdlib.h>
#include "Pos.h"
#include "util.h"

Pro2::Process2(): ProcessBase() {
}

Pro2::Process2(int w, int h): ProcessBase(w, h) {
  target.x = target.y = 0;
  target_d.x = target_d.y = 0;
}

int Pro2::isSelectedNextToTarget() {
  Pos p = table->getSelected();

  return ((p.x+1 == target.x || p.x-1 == target.x || p.x == target.x) && (p.y-1 == target.y || p.y+1 == target.y || p.y == target.y));
  return (p.x == target.x && (p.y == target.y-1 || p.y == target.y+1)) || (p.y == target.y && (p.x == target.x-1 || p.x == target.x+1));
}

void Pro2::moveSelectedNextTarget() {
  Pos distance;
  Pos dummy;
  // 既にソートした部分を壊さないように
  // 隣まで行ったらselectedを動かす方向にTargetを移動？
  dummy.x = distance.x = target.x - table->getSelected().x;
  dummy.y = distance.y = target.y - table->getSelected().y;
  dummy.x = abs(dummy.x);
  dummy.y = abs(dummy.y);

  if(target.y >= table->getSelected().y) {
    for(; dummy.y > 0 && !isSelectedNextToTarget(); dummy.y--) {
      if(distance.y < 0) table->swapSelected(UP);
      else table->swapSelected(DOWN);
    }
    for(; dummy.x > 0 && !isSelectedNextToTarget(); dummy.x--) {
      if(distance.x < 0) table->swapSelected(LEFT);
      else table->swapSelected(RIGHT);
    }
  } else {
    for(; dummy.x > 0 && !isSelectedNextToTarget(); dummy.x--) {
      if(distance.x < 0) table->swapSelected(LEFT);
      else table->swapSelected(RIGHT);
    }
    for(; dummy.y > 0 && !isSelectedNextToTarget(); dummy.y--) {
      if(distance.y < 0) table->swapSelected(UP);
      else table->swapSelected(DOWN);
    }
  }
}

// 間違ってるかも
void Pro2::moveSelectedToOppositeTarget(int direction) {
  int i;
  Pos p = table->getSelected();
  Pos dummy = target;

  dummy = surroundingsp(target, direction);
  if(checkPosEqual(dummy.x, dummy.y, p.x, p.y)) return;

  if(direction == UP && (surroundingsp(target, LEFT).x == p.x && target.y == p.y))
    table->swapSelected(DOWN);
  if(direction == DOWN && (surroundingsp(target, RIGHT).x == p.x && target.y == p.y))
    table->swapSelected(UP);
  if(direction == LEFT && (surroundingsp(target, UP).y == p.y && target.x == p.x))
    table->swapSelected(RIGHT);
  if(direction == RIGHT && (surroundingsp(target, DOWN).y == p.y && target.x == p.x))
    table->swapSelected(LEFT);

  if(direction == UP && (surroundingsp(target, LEFT).x == p.x && surroundingsp(target, DOWN).y == p.y)) {
    table->swapSelected(RIGHT);
  }
  if(direction == DOWN && (surroundingsp(target, RIGHT).x == p.x && surroundingsp(target, UP).y == p.y)) {
    table->swapSelected(LEFT);
  }
  if(direction == LEFT && (surroundingsp(target, RIGHT).x == p.x && surroundingsp(target, UP).y == p.y)) {
    table->swapSelected(DOWN);
  }
  if(direction == RIGHT && (surroundingsp(target, LEFT).x == p.x && surroundingsp(target, UP).y == p.y)) {
    table->swapSelected(RIGHT);
  }

  dummy = target;
  // 目的の位置より一番遠い時
  // 左右移動
  if(direction % 2 == 1 && p.y == dummy.y) {
    if(checkInScope(table->getWidth(), table->getHeight(), dummy.x, surroundingsp(dummy, DOWN).y)) table->swapSelected(DOWN);
    else if(checkInScope(table->getWidth(), table->getHeight(), dummy.x, surroundingsp(dummy, UP).y)) table->swapSelected(UP);
  }
  // 上下移動
  if(direction % 2 == 0 && p.x == dummy.x) {
    if(checkInScope(table->getWidth(), table->getHeight(), surroundingsp(dummy, RIGHT).x, dummy.y)) table->swapSelected(RIGHT);
    else if(checkInScope(table->getWidth(), table->getHeight(), surroundingsp(dummy, LEFT).x, dummy.y)) table->swapSelected(LEFT);
  }

  // 目的の位置の隣じゃない時
  p = table->getSelected();
  dummy = surroundingsp(target, direction);
  // 左右
  if(direction == RIGHT) {
    for(i = dummy.x - p.x; i > 0; i--) {
      table->swapSelected(direction);
    }
  } else if(direction == LEFT) {
    for(i = p.x - dummy.x; i > 0; i--) {
      table->swapSelected(direction);
    }
  } else if(direction == DOWN) {
    for(i = dummy.y - p.y; i > 0; i--) {
      table->swapSelected(direction);
    }
  } else if(direction == UP) {
    for(i = p.y - dummy.y; i > 0; i--) {
      table->swapSelected(direction);
    }
  }

  // 目的の位置の隣の時
  p = table->getSelected();
  dummy = surroundingsp(target, direction);
  if(p.x > dummy.x)
    table->swapSelected(LEFT);
  if(p.x < dummy.x)
    table->swapSelected(RIGHT);
  if(p.y > dummy.y)
    table->swapSelected(UP);
  if(p.y < dummy.y)
    table->swapSelected(DOWN);
}

int Pro2::moveTarget() {
  int direction, selected_direction;
  if(checkPosEqual(target_d.x, target_d.y, target.x, target.y)) return 1;
  // ターゲットの隣に移動
  moveSelectedNextTarget();

  direction = target_d.x < target.x ? LEFT : RIGHT;
  selected_direction = target_d.x < target.x ? RIGHT : LEFT;
  for(; target.x != target_d.x; ) {
    moveSelectedToOppositeTarget(direction);
    table->swapSelected(selected_direction);
    target = surroundingsp(target, direction);
  }
  direction = target_d.y < target.y ? UP : DOWN;
  selected_direction = target_d.y < target.y ? DOWN : UP ;
  for(; target.y != target_d.y; ) {
    moveSelectedToOppositeTarget(direction);
    table->swapSelected(selected_direction);
    target = surroundingsp(target, direction);
  }
  table->dispData();
  return 0;
}

void Pro2::sort() {
  int i, j;

  table->dispData();
  // 一番右下のデータを選択
  table->findAndSelectData(table->getWidth() - 1, table->getHeight() - 1);

  for(i = 0; i < table->getHeight() - 2; i++) {
    for(j = 0; j < table->getWidth() - 1; j++) {
      target_d.x = j;
      target_d.y = i;
      //table->dispData();
      target = table->findData(target_d.x, target_d.y);
      moveTarget();
    }
    puts("---------");
    target_d.x = table->getWidth() - 1;
    target_d.y = i;
    if(checkPosEqual(target.x, target.y, target_d.x, target_d.y)) continue;
    for(j = 0; j < table->getSelected().x; j++)
      table->swapSelected(LEFT);
    for(j = 0; j < table->getHeight() - i; j++)
      table->swapSelected(UP);
    for(j = 0; j < table->getSelected().x; j++)
      table->swapSelected(RIGHT);
    puts("asd");
    target = table->findData(target_d.x, target_d.y);
    moveTarget();
    table->swapSelected(LEFT);
    table->swapSelected(UP);
    for(j = 0; j < table->getWidth() - 2; j++)
      table->swapSelected(LEFT);
    table->swapSelected(DOWN);
    table->dispData();
    puts("===============");
  }
  table->dispCost();
}
