#ifndef INCLUDED_ID_DF_SEARCH_H
#define INCLUDED_ID_DF_SEARCH_H
#include "PosData.h"
#include <list>

using namespace std;

class ID_Data : public PosData {
  list<Pos> **adjacent;
  Pos selected;
  int md;
  int selected_num, changed_num;
  int move_flag;
  list<int> process;
  list<int> selected_x;
  list<int> selected_y;
  list<int> changed_nums;

private:
  ID_Data();
public:
  ID_Data(int w, int h);
  void calcMD();
  void findAndSelectData(Pos data);
  Pos findData(Pos data);
  int getChangedNum();
  int getLastMove();
  int getMD();
  string getStringSortData();
  void importData(PosData &import_data);
  void selectData(Pos p);
  int swapSelected(int direction);
  void undo();
};

#endif
