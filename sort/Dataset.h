#ifndef INCLUDED_DATASET_H
#define INCLUDED_DATASET_H
#include "Pos.h"

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
  int checkData(Pos **check_data);
  void deleteArray();
  void makeArray();
  void setDistance(int x, int y);
  void swapData(int x1, int y1, int x2, int y2);
  void swapNext(int x, int y, int direction);
public:
  Dataset(int w, int h);
  ~Dataset();
  void dispCost();
  void dispData(int x = -1, int y = -1);
  void dispDistance();
  int findData(int data_x, int data_y, int *x, int *y);
  int getWidth();
  int getHeight();
  void importData(Pos **import_data); // 未完成。使用不可
  void randomizeData();
  void resetData();
  void selectData(int x, int y);
  void swapSelected(int direction);
};

#endif
