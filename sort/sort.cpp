#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Process1.h"
#include "Process2.h"
#include "Process3.h"
#define WIDTH 6
#define HEIGHT 6

int main(void) {
  srand((unsigned)time(NULL));
  Process3 *test = new Process3(WIDTH, HEIGHT);
  test->sort();
  delete test;

  return 0;
}

