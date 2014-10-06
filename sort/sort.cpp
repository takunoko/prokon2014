#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Process1.h"
#include "Process2.h"
#include "Process3.h"
#include "Process4.h"
#include "Process5.h"
#define WIDTH 4
#define HEIGHT 5

int main(void) {
  srand((unsigned)time(NULL));
  Process5 *test = new Process5(WIDTH, HEIGHT);
  test->sort();
  delete test;

  return 0;
}

