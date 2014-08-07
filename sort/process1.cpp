#include <stdlib.h>
#include <stdio.h>
#include "func.h"
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
