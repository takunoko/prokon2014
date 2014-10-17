#ifndef INCLUDED_PROCESS6_H
#define INCLUDED_PROCESS6_H
#define LR 0
#define UD 1
#include "ProcessBase.h"
#include <list>

using namespace std;

typedef class Process6 : public ProcessBase {
private:
  Pos target;
  Pos target_data;
  list<Pos> sorted;

private:
  Process6();
  int calcParity();
  void dispSorted();
  void dispSortedData();
  int isSelectedNextToTarget();
  int isSorted(int y);
  void moveSelected(Pos destination);// なにか間違っているかも
  void moveSelectedNextTarget();
  int moveTarget(Pos pos);
  void rotateSelected(int direction);
  void sortDown();
  void sortUp();
public:
  Process6(int w, int h);
  string sort();
} Pro6;

#endif
