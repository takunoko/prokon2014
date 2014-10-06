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

int Pro4::isSorted(int y) {
  int x;
  Pos data;

  for(x = 0; x < table->getWidth(); x++) {
    data = table->getData(x, y);
    if(data.x != x || data.y != y) return 0;
  }
  return 1;
}

void Pro4::moveSelected(Pos destination) {
  Pos s = table->getSelected();
  list<Pos>::iterator p;

  int directionLR;
  int directionUD;
  int move_dir = 0;
  int move_flag = 0;
  int half;
  int old_direction = -1;

  puts("-------moveSelected-------");

  while(!checkPosEqual(destination, s)) {
    s = table->getSelected();
    //half = s.y / (table->getHeight() / 2);
    half = 1;
    directionLR = getDirectionLR(s.x, destination.x);
    directionUD = getDirectionUD(s.y, destination.y);
    table->dispData();
    if(half == 1) {
      if(directionLR == EQUAL) {
        move_dir = directionUD;
        move_flag = UD;
      } else {
        move_dir = directionLR;
        move_flag = LR;
      }
    } else if(half == 0) {
      if(directionUD == EQUAL) {
        move_dir = directionLR;
        move_flag = LR;
      } else {
        move_dir = directionUD;
        move_flag = UD;
      }
    }
    for(p = sorted.begin(); p != sorted.end(); p++) {
      if(checkPosEqual(surroundings(s, move_dir), *p)) {
        if(move_flag == LR) {
          move_dir = directionUD;
          move_flag = UD;
          break;
        } else if(move_flag == UD) {
          move_dir = directionLR;
          move_flag = LR;
          break;
        }
      }
    }
    for(p = sorted.begin(); p != sorted.end(); p++) {
      if(checkPosEqual(surroundings(s, move_dir), *p)) {
        if(move_flag == LR) {
          move_dir = getReversedDirection(directionLR);
          break;
        } else if(move_flag == UD) {
          move_dir = getReversedDirection(directionUD);
          break;
        }
      }
    }
    for(p = sorted.begin(); p != sorted.end(); p++) {
      if(checkPosEqual(surroundings(s, move_dir), *p)) {
        if(move_flag == LR) {
          move_dir = getReversedDirection(directionUD);
          move_flag = UD;
          break;
        } else if(move_flag == UD) {
          move_dir = getReversedDirection(directionLR);
          move_flag = LR;
          break;
        }
      }
    }
    /*
    if(old_direction == move_dir) {
      move_dir = getReversedDirection(move_dir);
    }*/
    printf("move_dir = %d\n", move_dir);
    table->swapSelected(move_dir);
    table->dispData(target.x, target.y);
    old_direction = getReversedDirection(move_dir);
  }
  puts("========moveSelected end=======");
}

void Pro4::moveSelectedNextTarget() {
  Pos s = table->getSelected();
  list<Pos>::iterator p;

  int directionLR;
  int directionUD;
  int move_dir = 0;
  int move_flag = 0;
  int half = 1;
  int old_direction = -1;

  puts("-------moveSelectedNextTarget-------");
  table->dispData(target.x, target.y);
  dispSorted();

  while(!isSelectedNextToTarget()) {
    s = table->getSelected();
    directionLR = getDirectionLR(s.x, target.x);
    directionUD = getDirectionUD(s.y, target.y);
    if(half == 1) {
      if(isNextX(target, s)) {
        move_dir = directionUD;
        move_flag = UD;
      } else {
        move_dir = directionLR;
        move_flag = LR;
      }
    } else if(half == 0) {
      if(isNextY(target, s)) {
        move_dir = directionLR;
        move_flag = LR;
      } else {
        move_dir = directionUD;
        move_flag = UD;
      }
    }
    for(p = sorted.begin(); p != sorted.end(); p++) {
      if(checkPosEqual(surroundings(s, move_dir), *p)) {
        if(move_flag == LR) {
          move_dir = directionUD;
          move_flag = UD;
          break;
        } else if(move_flag == UD) {
          move_dir = directionLR;
          move_flag = LR;
          break;
        }
      }
    }
    for(p = sorted.begin(); p != sorted.end(); p++) {
      if(checkPosEqual(surroundings(s, move_dir), *p)) {
        if(move_flag == LR) {
          move_dir = getReversedDirection(directionLR);
          break;
        } else if(move_flag == UD) {
          move_dir = getReversedDirection(directionUD);
          break;
        }
      }
    }
    for(p = sorted.begin(); p != sorted.end(); p++) {
      if(checkPosEqual(surroundings(s, move_dir), *p)) {
        if(move_flag == LR) {
          move_dir = getReversedDirection(directionUD);
          move_flag = UD;
          break;
        } else if(move_flag == UD) {
          move_dir = getReversedDirection(directionLR);
          move_flag = LR;
          break;
        }
      }
    }
    /*
    for(p = sorted.begin(); p != sorted.end(); p++) {
      if(checkPosEqual(surroundings(s, move_dir), *p)) {
        if(move_flag == LR) {
          move_dir = directionUD;
          move_flag = UD;
          break;
        } else if(move_flag == UD) {
          move_dir = directionLR;
          move_flag = LR;
          break;
        }
      }
    }
    if(old_direction == move_dir) {
      move_dir = getReversedDirection(move_dir);
    }
    for(p = sorted.begin(); p != sorted.end(); p++) {
      if(checkPosEqual(surroundings(s, move_dir), *p)) {
        if(move_flag == LR) {
          move_dir = getReversedDirection(directionUD);
          move_flag = UD;
          break;
        } else if(move_flag == UD) {
          move_dir = getReversedDirection(directionLR);
          move_flag = LR;
          break;
        }
      }
    }*/
    table->swapSelected(move_dir);
    table->dispData(target.x, target.y);
    old_direction = getReversedDirection(move_dir);
  }
  puts("=======moveSelectedNextTarget=======");
}

int Pro4::moveTarget(Pos pos) {
  list<Pos>::iterator p;

  int directionLR;
  int directionUD;
  int move_dir = 0;
  int move_flag = 0;
  int half = target.y / (table->getHeight() / 2);
  int old_direction = -1;
  int old_flag = -1;

  puts("-------moveTarget-------");

  while(!checkPosEqual(target, pos)) {
    target = table->findData(target_data);
    directionLR = getDirectionLR(target.x, pos.x);
    directionUD = getDirectionUD(target.y, pos.y);
    table->dispData();
    if(old_flag == LR) {
      if(directionUD == EQUAL) {
        move_dir = directionLR;
        move_flag = LR;
      } else {
        move_dir = directionUD;
        move_flag = UD;
      }
    } else {
      if(directionLR == EQUAL) {
        move_dir = directionUD;
        move_flag = UD;
      } else {
        move_dir = directionLR;
        move_flag = LR;
      }
    }
    for(p = sorted.begin(); p != sorted.end(); p++) {
      if(checkPosEqual(surroundings(target, move_dir), *p)) {
        if(move_flag == LR) {
          move_dir = directionUD;
          move_flag = UD;
          break;
        } else if(move_flag == UD) {
          move_dir = directionLR;
          move_flag = LR;
          break;
        }
      }
    }
    if(old_direction == move_dir) {
      move_dir = getReversedDirection(move_dir);
    }
    for(p = sorted.begin(); p != sorted.end(); p++) {
      if(checkPosEqual(surroundings(target, move_dir), *p)) {
        if(move_flag == LR) {
          move_dir = getReversedDirection(directionUD);
          move_flag = UD;
          break;
        } else if(move_flag == UD) {
          move_dir = getReversedDirection(directionLR);
          move_flag = LR;
          break;
        }
      }
    }
    printf("directionaaaaa%d, %d, %d\n", directionLR, directionUD, move_dir);
    moveSelectedNextTarget();
    rotateSelected(move_dir);
    table->swapSelected(getReversedDirection(move_dir));
    table->dispData(target.x, target.y);
    old_direction = getReversedDirection(move_dir);
    old_flag = move_flag;
  }
  puts("=======moveTarget end========");
  return 0;
}

void Pro4::rotateSelected(int direction) {
  // directionは動かす方向
  // targetから見たselectedの方向
  int dir_selected = getDirection(target, table->getSelected());
  int move_distance = abs(direction - dir_selected);
  // 1か-1
  // 間違ってる
  //-----------------------------直すところ
  int move_direction = (dir_selected > direction) ? -1 : 1;

  puts("-------rotateSelected-------");
  printf("direction = %d\n", direction);
  printf("target = %d, %d\n", target.x, target.y);
  if(direction == EQUAL) return;
  if(checkPosEqual(table->getSelected(), surroundings(target, direction)))  {
    puts("selected dont need to rotate");
    return;
  }
  if(!checkInScope(table->getWidth(), table->getHeight(), surroundings(target, direction).x, surroundings(target, direction).y)) {
    puts("cant rotate(move target)...");
    return;
  }

  if(move_distance > DIRECTION_NUM / 2) {
    puts("rotate distance is too long");
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
    if(reverse_flag) {
      move_distance = DIRECTION_NUM - move_distance;
      move_direction *= -1;
      break;
    }
  }

  int move_dir;
  int j = dir_selected;
  printf("move_direction = %d\n", move_direction);
  printf("move_distance = %d\n", move_distance);
  for(i = 0; i < move_distance; i++) {
    // 目的地についたら止めたほうがいいかも
    j = (j + move_direction + DIRECTION_NUM) % DIRECTION_NUM;
    move_dir = getDirection(table->getSelected(), surroundings(target, j));
    printf("j = %d\n", j);
    printf("move_dir = %d\n", move_dir);
    table->swapSelected(move_dir);
    table->dispData(target.x, target.y);
  }
  puts("======rotate end========");
}

void Pro4::sort() {
  table->dispData();
  // 一番右下になるデータを選択
  table->findAndSelectData(table->getWidth()-1, table->getHeight()-1);
  table->dispData();
  /*target = table->findData(0, 0);
  moveSelectedNextTarget();
  rotateSelected(UP);

  return;
  */
  sortUp();
  table->dispData();
  table->dispCost();

  // 下二列
}

void Pro4::sortUp() {
  int i, j;
  // 上半分
  for(i = 0; i < table->getHeight() - 2; i++) {
    // 端以外の
    if(isSorted(i)) {
      puts("continue1");
      for(j = 0; j < table->getWidth(); j++) {
        sorted.push_back(Pos(j, i));
      }
      continue;
    }
    for(j = 0; j < table->getWidth() - 2; j++) {
      target_data = Pos(j, i);
      target = table->findData(target_data);
      table->dispData(target.x, target.y);
      // 目的地へ移動
      moveTarget(target_data);
      target = table->findData(target_data);
      sorted.push_back(target);
      puts("sorted");
    }
    // もし、1列全部揃ってたらcontinue
    if(isSorted(i)) {
      puts("continue2");
      sorted.push_back(Pos(table->getWidth()-2, i));
      sorted.push_back(Pos(table->getWidth()-1, i));
      continue;
    }
    Pos dummy = table->findData(Pos(table->getWidth()-2, i));
    if(dummy.x >= table->getWidth()-2 && dummy.x < table->getWidth() && dummy.y >= i && dummy.y < i+2) {
      target_data = Pos(table->getWidth()-2, i);
      target = table->findData(target_data);
      moveTarget(Pos(table->getWidth()-2, i+2));
    }
    // もし，width-2のところに20が来てしまった時は20を下に移す
    if(!checkPosEqual(table->getData(table->getWidth()-2, i), Pos(table->getWidth()-1, i)) || !checkPosEqual(table->getData(table->getWidth()-2, i+1), Pos(table->getWidth()-2, i))) {
      target_data = Pos(table->getWidth()-1, i);
      target = table->findData(target_data);
      moveTarget(Pos(table->getWidth()-2, i));
      sorted.push_back(Pos(table->getWidth()-2, i));
      target_data = Pos(table->getWidth()-2, i);
      target = table->findData(target_data);
      moveTarget(Pos(table->getWidth()-2, i+1));
      sorted.push_back(Pos(table->getWidth()-2, i+1));
    } else {
    }
    moveSelected(Pos(table->getWidth()-1, i));
    table->swapSelected(LEFT);
    table->swapSelected(DOWN);
    sorted.pop_back();
    sorted.pop_back();
    sorted.push_back(Pos(table->getWidth()-2, i));
    sorted.push_back(Pos(table->getWidth()-1, i));
    table->dispData();
  }
}
