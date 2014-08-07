#include <stdlib.h>
#include <stdio.h>
#include "util.h"
#include "Dataset.h"
#include "Process1.h"

Pro1::Process1() {
}

Pro1::Process1(int width, int height) {
  table = new Dataset(width, height);
  table->randomizeData();
  table->dispDistance();
  table->dispData();
  sort();
  table->dispCost();
}

Pro1::~Process1() {
  delete table;
}

void Pro1::sort() {
  int i, j, k, originX = 0, originY = 0;

  for(i = 0; i < table->getHeight(); i++) {
    for(j = 0; j < table->getWidth(); j++) {
      if(table->findData(j, i, &originX, &originY)) exit(1);
      table->selectData(originX, originY);
      table->dispData();
      if(originX > j) {
        for(k = originX; k != j; k--) {
          table->swapSelected(LEFT);
        }
      } else {
        for(k = originX; k != j; k++) {
          table->swapSelected(RIGHT);
        }
      }
      if(originY > i) {
        for(k = originY; k != i; k--) {
          table->swapSelected(UP);
        }
      } else {
        for(k = originY; k != i; k++) {
          table->swapSelected(DOWN);
        }
      }
    }
  }
}
