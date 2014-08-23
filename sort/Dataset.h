#ifndef INCLUDED_DATASET_H
#define INCLUDED_DATASET_H
#define MAXWIDTH 16 // 0x10
#define MAXHEIGHT 16
#include "Pos.h"
#include "PosData.h"

class Dataset {
private:
  Pos **data;
  Pos **distance;
  Pos selected;
  int width, height;
  int selected_num, changed_num;
  int move_flag;

private:
  Dataset();
  int checkData(PosData check_data);
  void deleteArray();
  void makeArray();
  void setDistance(int x, int y);
  void swapData(int x1, int y1, int x2, int y2);
  void swapNext(int x, int y, int direction);
public:
  Dataset(int w, int h);
  ~Dataset();
  int checkSorted();
  void dispCost();
  void dispData(int x = -1, int y = -1);
  void dispDistance();
  void findAndSelectData(int x, int y);
  void findAndSelectData(Pos data);
  Pos findData(int data_x, int data_y);
  Pos findData(Pos data);
  Pos getData(int x, int y);
  Pos getDistance(int x, int y);
  int getHeight();
  int getWidth();
  Pos getSelected();
  Pos getSelectedData();
  Pos getSelectedDistance();
  int getX(int ox, int oy);
  int getY(int ox, int oy);
  void importData(PosData import_data);
  void randomizeData();
  void resetData();
  void selectData(int x, int y);
  void swapSelected(int direction);
};

#endif
