#ifndef INCLUDED_DATASET_H
#define INCLUDED_DATASET_H
#include "Pos.h"

class PosData {
protected:
  Pos **data;
  int width, height;

public:
  PosData();
  PosData(int w, int h);
  ~PosData();
  void dispData();
  Pos **getData();
  void setX(int ox, int oy, int x);
  void setY(int ox, int oy, int y);
  void setData(int ox, int oy, int x, int y);
};

#endif
