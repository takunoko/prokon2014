#include <stdio.h>
#include "ProcessBase.h"

ProBase::ProcessBase() {
}

ProBase::ProcessBase(int width, int height) {
  table = new Dataset(width, height);
  table->randomizeData();
  max_select = 2;
  select_cost = -1;
  change_cost = -1;
}

ProBase::ProcessBase(int width, int height, int max_select, int select_cost, int change_cost) {
  table = new Dataset(width, height);
  table->randomizeData();
  this->max_select = max_select;
  this->select_cost = select_cost;
  this->change_cost = change_cost;
}

ProBase::~ProcessBase() {
  delete table;
}

void ProBase::importData(PosData &data) {
  table->importData(data);
}
