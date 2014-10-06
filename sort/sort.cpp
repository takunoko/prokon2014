#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Process1.h"
#include "Process2.h"
#include "Process3.h"
#include "Process4.h"
#define WIDTH 4
#define HEIGHT 5

int main(void) {
  srand((unsigned)time(NULL));
  Process4 *test = new Process4(WIDTH, HEIGHT);
  test->sort();
  delete test;

  return 0;
}

