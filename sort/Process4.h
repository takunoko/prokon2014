#ifndef INCLUDED_PROCESS4_H
#define INCLUDED_PROCESS4_H
#define LR 0
#define UD 1
#include "ProcessBase.h"
#include <list>

using namespace std;

typedef class Process4 : public ProcessBase {
private:
  Pos target;
  Pos target_data;
  list<Pos> sorted;

private:
  Process4();
  void dispSorted();
  int isSelectedNextToTarget();
  void moveSelected(Pos destination);
  void moveSelectedNextTarget();
  int moveTarget(Pos pos);
  void rotateSelected(int direction);
public:
  Process4(int w, int h);
  void sort();
} Pro4;

// 指定した座標に，sortedを使いながらselectedを移動するメソッドを追加する．

#endif
