#ifndef INCLUDED_MDATASET_H
#define INCLUDED_MDATASET_H
#define MAXWIDTH 17 // 0x10
#define MAXHEIGHT 17
#include "Pos.h"
#include "PosData.h"
#include <list>

using namespace std;

class MDataset {
private:
  Pos **data;
  Pos selected;
  int mdistance;
  int width, height;
  int selected_num, changed_num;
  int move_flag;
  list<int> process;
  list<int> selected_x;
  list<int> selected_y;
  list<int> changed_nums;

private:
  MDataset();
  void deleteArray();
  void makeArray();
  void setDistance(int x, int y);
  void swapData(int x1, int y1, int x2, int y2);
  void swapNext(int x, int y, int direction);
public:
  MDataset(int w, int h);
  ~MDataset();
  void calcMD();
  int checkSorted();
  void dispCost();
  void dispData(int x = -1, int y = -1);
  void findAndSelectData(int x, int y);
  void findAndSelectData(Pos data);
  Pos findData(int data_x, int data_y);
  Pos findData(Pos data);
  int getChangedNum();
  Pos getData(int x, int y);
  int getDistance();
  int getHeight();
  int getLastMove();
  int getWidth();
  Pos getSelected();
  Pos getSelectedData();
  string getStringSortData();
  int getX(int ox, int oy);
  int getY(int ox, int oy);
  void importData(PosData &import_data);
  void randomizeData();
  void resetData();
  void selectData(int x, int y);
  int swapSelected(int direction);
  void undo();
};

#endif
