#include <stdlib.h>
#include <stdio.h>
#include "util.h"
#include "dataset.h"
#include "process1.h"

pro1::process1() {
}

pro1::process1(int width, int height) {
  table = new dataset(width, height);
  make_data();
  table->disp_data();
  table->disp_distance();
}

pro1::~process1() {
  delete table;
}

// 乱数でデータをかき回す
void pro1::make_data() {
  int i, j, x, y, w, h;

  w = table->get_width();
  h = table->get_height();
  table->reset_data();

  for(i = 0; i < h; i++) {
    for(j = 0; j < w; j++) {
      x = rand() % w;
      y = rand() % h;
      table->swap_data(j, i, x, y);
    }
  }
}

void pro1::sort() {
  int i, j, k, originX = 0, originY = 0;

  for(i = 0; i < table->get_height(); i++) {
    for(j = 0; j < table->get_width(); j++) {
      if(table->find_data(j, i, &originX, &originY)) exit(1);
      table->disp_data(originX, originY);
      if(originX > j) {
        for(k = originX; k != j; k--) {
          table->swap_next(k, originY, LEFT);
        }
      } else {
        for(k = originX; k != j; k++) {
          table->swap_next(k, originY, RIGHT);
        }
      }
      if(originY > i) {
        for(k = originY; k != i; k--) {
          table->swap_next(j, k, UP);
        }
      } else {
        for(k = originY; k != i; k++) {
          table->swap_next(j, k, DOWN);
        }
      }
    }
  }
}
