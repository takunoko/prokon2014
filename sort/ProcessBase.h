#ifndef INCLUDED_PROCESSBASE_H
#define INCLUDED_PROCESSBASE_H
#include "dataset.h"

typedef class ProcessBase {
protected:
  Dataset *table;
  int max_select;
  int select_cost;
  int change_cost;

protected:
  ProcessBase();
public:
  ProcessBase(int width, int height);
  ProcessBase(int width, int height, int max_select, int select_cost, int change_cost);
  ~ProcessBase();
  void importData(PosData &data);
} ProBase;

#endif
