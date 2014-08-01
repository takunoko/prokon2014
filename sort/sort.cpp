#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "process1.h"
#define MAXCOL 16 // 0x10
#define MAXROW 16
#define COL 3
#define ROW 3

int main(void) {
  process1 test = process1(COL, ROW);
  return 0;
}

