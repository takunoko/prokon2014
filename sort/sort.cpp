#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Dataset.h"
#include "Process1.h"
#include "Process2.h"
#include "Process3.h"
#include "Process4.h"
#include "Process5.h"
#define WIDTH 3
#define HEIGHT 3

int main(void) {
  PosData *data = new PosData(WIDTH, HEIGHT);
  string data_send;
  int i, j, x, y;

  srand((unsigned)time(NULL));

  for(i = 0; i < data->getHeight(); i++) {
    for(j = 0; j < data->getWidth(); j++) {
      x = rand() % data->getWidth();
      y = rand() % data->getHeight();
      data->setData(i, j, i, j);
    }
  }
  Process5 *test = new Process5(WIDTH, HEIGHT);
  test->importData(*data);
  data_send = test->sort();
  delete test;

  return 0;
}

