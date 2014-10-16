#ifndef INCLUDED_POSDATA_H
#define INCLUDED_POSDATA_H
#include "Pos.h"

class PosData {
private:
  Pos **data;
  int width, height;

public:
  PosData();
  PosData(int w, int h);
  ~PosData();
  void dispData();
  int getHeight();
  int getWidth();
  int getX(int ox, int oy);
  int getY(int ox, int oy);
  void randomizeData();
  void setX(int ox, int oy, int x);
  void setY(int ox, int oy, int y);
  void setData(int ox, int oy, int x, int y);
  void swapData(int x1, int y1, int x2, int y2);
  void swapData(int p1, int p2);
};

#endif
