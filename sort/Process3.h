#ifndef INCLUDED_PROCESS3_H
#define INCLUDED_PROCESS3_H
#include "ProcessBase.h"
#include <list>

typedef class Process3 : public ProcessBase {
private:
  Pos target;
  Pos target_data;
  std::list<Pos> sorted;

private:
  Process3();
  int isSelectedNextToTarget();
  void moveSelectedNextTarget();
  void moveSelectedToOppositeTarget(int direction);
  int moveTarget();
public:
  Process3(int w, int h);
  void sort();
} Pro3;

#endif
