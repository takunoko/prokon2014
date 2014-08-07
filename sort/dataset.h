#ifndef INCLUDED_DATASET_H
#define INCLUDED_DATASET_H
#include "pos.h"

class dataset {
private:
  pos **data;
  pos **distance;
  pos selected;
  int width, height;
  int selected_num, changed_num;
  int move_flag;

private:
  dataset();
  void delete_array();
  void make_array();
  void set_distance(int x, int y);
  void swap_data(int x1, int y1, int x2, int y2);
  void swap_next(int x, int y, int direction);
public:
  dataset(int w, int h);
  ~dataset();
  void disp_data(int x = -1, int y = -1);
  void disp_distance();
  void disp_cost();
  int find_data(int data_x, int data_y, int *x, int *y);
  int get_width();
  int get_height();
  void randomize_data();
  void reset_data();
  void select_data(int x, int y);
  void swap_selected(int direction);
};

#endif
