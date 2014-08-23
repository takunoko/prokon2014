#include <stdio.h>
#include <stdlib.h>
#include "Process3.h"

using namespace std;

Pro3::Process3(): ProcessBase() {
  target.setZero();
  target_data.setZero();
}

Pro3::Process3(int w, int h): ProcessBase(w, h) {
  target.setZero();
  target_data.setZero();
}

// 安全
int Pro3::isSelectedNextToTarget() {
  Pos p = table->getSelected();

  return ((p.x+1 == target.x || p.x-1 == target.x || p.x == target.x) && (p.y-1 == target.y || p.y+1 == target.y || p.y == target.y));
}

int Pro3::moveTarget() {
  iterator p;
}

void Pro3::sort() {
  int i, j;

  table->dispData();
  // 一番右下になるデータを選択
  table->findAndSelectData(table->getWidth()-1, table->getHeight()-1);

  // 上半分
  for(i = 0; i < table->getHeight() - 2; i++) {
    // 端以外の
    for(j = 0; j < table->getWidth() - 1; j++) {
      target_data = Pos(j, i);
      target = table->findData(target_data);
      table->dispData(target.x, target.y);
      // targetの隣へ
      moveTarget();
      // 目的地へ移動
      sorted.push_back(target);
    }
    // 一番右側
    // ソートした部分をずらす
    // 一番右側のを正しい位置に
    // ずらしたのを戻す
    sorted.push_back(target);
  }

  // 下二列
}
