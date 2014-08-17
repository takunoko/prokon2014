#ifndef INCLUDED_PROCESS2_H
#define INCLUDED_PROCESS2_H
#include "ProcessBase.h"

typedef class Process2: public ProcessBase {
private:
  int target_x, target_y;
  int target_dx, target_dy; // target_data_x, y
private:
  Process2();
public:
  Process2(int w, int h);
  void moveSelectedNextTarget();
  void moveTarget();
  void sort();
} Pro2;

#endif
