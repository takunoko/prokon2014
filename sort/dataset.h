#ifndef INCLUDED_DATASET_H
#define INCLUDED_DATASET_H
#include "pos.h"

class dataset {
private:
  pos **data;
  pos **distance;
  int width, height;

public:
  dataset();
  dataset(int w, int h);
  ~dataset();
  void delete_array();
  void disp_data(int x = -1, int y = -1);
  void disp_distance(int x = -1, int y = -1);
  int find_data(int data_x, int data_y, int *x, int *y);
  int get_width();
  int get_height();
  void make_array();
  void reset_data();
  void set_distance(int x, int y);
  void swap_data(int x1, int y1, int x2, int y2);
  void swap_next(int x, int y, int direction);
};

#endif
