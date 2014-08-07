#ifndef INCLUDED_PROCESS1_H
#define INCLUDED_PROCESS1_H
#include "dataset.h"

typedef class process1 {
public:
  dataset *table;

  process1();
  process1(int width, int height);
  ~process1();
  void make_data();
  void sort();
} pro1;

#endif
