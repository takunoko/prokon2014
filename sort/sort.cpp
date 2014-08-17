#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Process1.h"
#include "Process2.h"
#define WIDTH 4
#define HEIGHT 4

int main(void) {
  srand((unsigned)time(NULL));
  Process2 *test = new Process2(WIDTH, HEIGHT);
  test->sort();
  delete test;

  return 0;
}

