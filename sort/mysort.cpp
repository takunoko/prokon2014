#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Process1.h"
#include "Process2.h"
#include "Process3.h"
#include "Process4.h"
#include "Process5.h"
#include "Process6.h"
#include "Process8.h"
#define WIDTH 2
#define HEIGHT 3

int main(void) {
  PosData *data = new PosData(WIDTH, HEIGHT);
  string data_send;
  int i, j, x, y;

  srand((unsigned)time(NULL));

  /*
  for(i = 0; i < data->getHeight(); i++) {
    for(j = 0; j < data->getWidth(); j++) {
      data->setData(j, i, j, i);
    }
  }
  for(i = 0; i < data->getHeight(); i++) {
    for(j = 0; j < data->getWidth(); j++) {
      x = rand() % data->getWidth();
      y = rand() % data->getHeight();
      data->swapData(j, i, x, y);
    }
  }*/

  // ソート
  Process8 *test = new Process8(WIDTH, HEIGHT);
  //test->importData(*data);
  data_send = test->sort();
  //test->sort();
  delete test;
  cerr << data_send << endl;

  return 0;
}

