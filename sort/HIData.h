#ifndef INCLUDED_HIDATA_H
#define INCLUDED_HIDATA_H
#include "PosData.h"
#include <list>

using namespace std;

class HIData {
public:
  int *data;
  int *distance;
  list<int> *adjacent;
  int selected;
  int width, height;
  int md;
  int selected_num, changed_num;
  int move_flag;
  list<int> process;
  list<int> selected_x;
  list<int> selected_y;
  list<int> changed_nums;

private:
  HIData();
public:
  HIData(int w, int h);
  void calcMD();
  void dispData();
  void findAndSelectData(int data);
  int findData(int p);
  int getChangedNum();
  int getLastMove();
  int getMD();
  Pos getSelected();
  string getStringSortData();
  void importData(PosData &import_data);
  void selectData(int p);
  void swapData(int p1, int p2);
  int swapSelected(int pos);
  void undo();
};

#endif
