#ifndef INCLUDED_PROCESS8_H
#define INCLUDED_PROCESS8_H
#include "id_df_search.h"

typedef class Process8 {
  int lower_bound;
  int start_bound;
  int isCleared;
  int limit;
  int move;
protected:
  ID_Data *table;

public:
  Process8(int width, int height);
  ~Process8();
  int calcParity();
  void importData(PosData &data);
  Process8();
  string sort();
  int id_search();
} Pro8;

#endif
