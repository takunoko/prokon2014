#ifndef INCLUDED_PROCESS2_H
#define INCLUDED_PROCESS2_H
#include "ProcessBase.h"

typedef class Process2: public ProcessBase {
private:
  Pos target;
  Pos target_d; // target_data_x, y

private:
  Process2();
  int isSelectedNextToTarget();
  void moveSelectedNextTarget();
  void moveSelectedToOppositeTarget(int direction);
  int moveTarget();
public:
  Process2(int w, int h);
  void sort();
} Pro2;

#endif
