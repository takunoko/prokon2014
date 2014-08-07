#include <stdlib.h>
#include <stdio.h>
#include "util.h"
#include "dataset.h"
#include "process1.h"

pro1::process1() {
}

pro1::process1(int width, int height) {
  table = new dataset(width, height);
  table->randomize_data();
  table->disp_distance();
  table->disp_data();
  sort();
  table->disp_cost();
}

pro1::~process1() {
  delete table;
}

void pro1::sort() {
  int i, j, k, originX = 0, originY = 0;

  for(i = 0; i < table->get_height(); i++) {
    for(j = 0; j < table->get_width(); j++) {
      if(table->find_data(j, i, &originX, &originY)) exit(1);
      table->select_data(originX, originY);
      table->disp_data();
      if(originX > j) {
        for(k = originX; k != j; k--) {
          table->swap_selected(LEFT);
        }
      } else {
        for(k = originX; k != j; k++) {
          table->swap_selected(RIGHT);
        }
      }
      if(originY > i) {
        for(k = originY; k != i; k--) {
          table->swap_selected(UP);
        }
      } else {
        for(k = originY; k != i; k++) {
          table->swap_selected(DOWN);
        }
      }
    }
  }
}
