#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "process1.h"
#define MAXWIDTH 16 // 0x10
#define MAXHEIGHT 16
#define WIDTH 3
#define HEIGHT 3

int main(void) {
  process1 *test = new process1(WIDTH, HEIGHT);
  delete test;

  return 0;
}

