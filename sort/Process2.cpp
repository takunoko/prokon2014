#include "Process2.h"
#include "util.h"

Pro2::Process2(): ProcessBase() {
}

Pro2::Process2(int w, int h): ProcessBase(w, h) {
  target_x = target_y = 0;
}

void Pro2::moveSelectedNextTarget() {
  // 既にソートした部分を壊さないように
  // 
}

void Pro2::moveTarget() {
  // ターゲットの隣に移動
  moveSelectedNextTarget();
  // 縦移動
  // 横移動
}

void Pro2::sort() {
  int i, j;

  table->dispData();
  // 一番右下のデータを選択
  table->findAndSelectData(table->getWidth() - 1, table->getHeight() - 1);

  for(i = 0; i < table->getHeight(); i++) {
    for(j = 0; j < table->getWidth(); j++) {
      target_dx = j;
      target_dy = i;
      table->dispData();
      table->findData(j, i, &target_x, &target_y);
      if(checkPosEqual(j, i, target_x, target_y)) break;
      moveTarget();
    }
  }
}
