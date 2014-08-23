#include <stdlib.h>
#include "util.h"
#include "Process1.h"

Pro1::Process1(): ProcessBase() {
}

Pro1::Process1(int width, int height): ProcessBase(width, height) {
}

Pro1::~Process1() {
}

void Pro1::sort() {
  int i, j, k;
  Pos origin;
  origin.x = 0;
  origin.y = 0;
  // originは動かす前の場所

  for(i = 0; i < table->getHeight(); i++) {
    for(j = 0; j < table->getWidth(); j++) {
      table->selectData(origin.x, origin.y);
      table->dispData();
      if(origin.x > j) {
        for(k = origin.x; k != j; k--) {
          table->swapSelected(LEFT);
        }
      } else {
        for(k = origin.x; k != j; k++) {
          table->swapSelected(RIGHT);
        }
      }
      if(origin.y > i) {
        for(k = origin.y; k != i; k--) {
          table->swapSelected(UP);
        }
      } else {
        for(k = origin.y; k != i; k++) {
          table->swapSelected(DOWN);
        }
      }
    }
  }
  table->dispCost();
}
