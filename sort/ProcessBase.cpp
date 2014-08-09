#include "ProcessBase.h"
#include <stdio.h>

ProBase::ProcessBase() {
}

ProBase::ProcessBase(int width, int height) {
  table = new Dataset(width, height);
  table->randomizeData();
  table->dispDistance();
  table->dispData();
}

ProBase::~ProcessBase() {
  delete table;
}
