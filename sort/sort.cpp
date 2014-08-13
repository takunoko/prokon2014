#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Process1.h"
#include "Process2.h"
#define MAXWIDTH 16 // 0x10
#define MAXHEIGHT 16
#define WIDTH 3
#define HEIGHT 3

int main(void) {
  srand((unsigned)time(NULL));
  Process1 *test = new Process1(WIDTH, HEIGHT);
  test->sort();
  delete test;

  return 0;
}

