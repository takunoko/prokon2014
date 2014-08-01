#ifndef INCLUDED_PROCESS1_H
#define INCLUDED_PROCESS1_H
#include "dataset.h"

typedef class process1 {
public:
  dataset table;

  process1();
  process1(int size_x, int size_y);
  void make_data();
} pro1;

#endif
