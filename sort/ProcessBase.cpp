#include "ProcessBase.h"

ProBase::ProcessBase() {
}

ProBase::ProcessBase(int width, int height) {
  table = new Dataset(width, height);
  table->randomizeData();
}

ProBase::~ProcessBase() {
  delete table;
}

void ProBase::importData(PosData data) {
  table->importData(data);
}
