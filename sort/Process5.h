#ifndef INCLUDED_PROCESS5_H
#define INCLUDED_PROCESS5_H
#define LR 0
#define UD 1
#include "ProcessBase.h"
#include <list>

using namespace std;

typedef class Process5 : public ProcessBase {
private:
  Pos target;
  Pos target_data;
  list<Pos> sorted;

private:
  Process5();
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
  Process5(int w, int h);
  string sort();
} Pro5;

// 指定した座標に，sortedを使いながらselectedを移動するメソッドを追加する．

#endif
