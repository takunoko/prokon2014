#ifndef INCLUDED_PROCESS2_H
#define INCLUDED_PROCESS2_H
#include "ProcessBase.h"

typedef class Process2: public ProcessBase {
private:
  Process2();
public:
  Process2(int w, int h);
  void sort();
} Pro2;

#endif
