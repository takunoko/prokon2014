#ifndef INCLUDED_PROCESS1_H
#define INCLUDED_PROCESS1_H
#include "ProcessBase.h"

typedef class Process1: public ProcessBase {
  Process1();
public:
  Process1(int width, int height);
  ~Process1();
  void sort();
} Pro1;

#endif
