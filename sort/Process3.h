#ifndef INCLUDED_PROCESS3_H
#define INCLUDED_PROCESS3_H
#include "ProcessBase.h"
#include <list>

using namespace std;

typedef class Process3 : public ProcessBase {
private:
  Pos target;
  Pos target_data;
  list<Pos> sorted;

private:
  Process3();
  void dispSorted();
  int isSelectedNextToTarget();
  void moveSelectedNextTarget();
  int moveTarget();
  void rotateSelected(int direction);
public:
  Process3(int w, int h);
  void sort();
} Pro3;

#endif
