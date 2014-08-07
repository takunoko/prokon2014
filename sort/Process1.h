#ifndef INCLUDED_PROCESS1_H
#define INCLUDED_PROCESS1_H
#include "dataset.h"

typedef class Process1 {
public:
  Dataset *table;

  Process1();
  Process1(int width, int height);
  ~Process1();
  void sort();
} Pro1;

#endif
