#ifndef INCLUDED_PROCESSBASE_H
#define INCLUDED_PROCESSBASE_H
#include "dataset.h"

typedef class ProcessBase {
protected:
  Dataset *table;

protected:
  ProcessBase();
public:
  ProcessBase(int width, int height);
  ~ProcessBase();
  void importData(PosData &data);
} ProBase;

#endif
