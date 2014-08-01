#include <stdlib.h>
#include "func.h"
#include "dataset.h"
#include "process1.h"

pro1::process1() {
}

pro1::process1(int size_y, int size_x) {
  table = dataset(size_y, size_x);
  make_data();
  table.disp_data();
  table.disp_distance();
}

// 乱数でデータをかき回す
void pro1::make_data() {
  int i, j, r, c;

  table.reset_data();

  for(i = 0; i < table.size_x; i++) {
    for(j = 0; j < table.size_y; j++) {
      r = rand() % table.size_x;
      c = rand() % table.size_y;
      table.swap_data(i, j, r, c);
    }
  }
}
