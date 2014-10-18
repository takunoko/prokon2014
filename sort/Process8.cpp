#include <stdlib.h>
#include "util.h"
#include "Process8.h"

Pro8::Process8() {
}

Pro8::Process8(int width, int height) {
  table = new ID_Data(width, height);
  table->randomizeData();
}

Pro8::~Process8() {
  delete table;
}
int Pro8::calcParity() {
  int sum = 0;
  int num = 0;
  int i, j;
  Pos data, data2;

  for(i = 0; i < table->getWidth()*table->getHeight(); i++) {
    num = 0;
    data2 = table->getData(Pos(i%table->getWidth(), i/table->getWidth()));
    for(j = i; j < table->getWidth()*table->getHeight(); j++) {
      data = table->getData(Pos(j%table->getWidth(), j/table->getWidth()));
      if(data2.x+data2.y*table->getWidth() > (data.x+data.y*table->getWidth())) {
        num++;
      }
    }
    sum += num;
  }
  return sum % 2;
}


int Pro8::id_search() {
  //printf("D%d,%d\n", table->getMD(), lower_bound);
  //printf("S%d,%d\n", table->getChangedNum(), move);
  if(table->getChangedNum() > limit) {
    printf("%d\n", table->getMD());
    table->dispData();
    if(table->getMD() <= 1) {
      isCleared = 1;
      return 1;
    }
  }else {
    if(table->getChangedNum()+table->getMD() < lower_bound) {
      for(list<int>::iterator i = table->adjacent[table->selected.y][table->selected.x].begin(); i != table->adjacent[table->selected.y][table->selected.x].end(); i++) {
        if(getReversedDirection(table->getLastMove()) == *i) {
          continue;
        }
        if(table->swapSelected(*i)) {
          continue;
        }
        int is= id_search();
        if(is) {
          return 1;
        }
        table->undo();
      }
    }
  }
  return 0;
}

void Pro8::importData(PosData &data) {
  table->importData(data);
}

string Pro8::sort() {
  limit = 31;

  table->findAndSelectData(Pos(table->getWidth()-1, table->getHeight()-1));
  table->calcMD();
  //if(calcParity()) return "";

  lower_bound = table->getMD();
  start_bound = table->getMD()%2;
  printf("D%d\n", table->getMD());
  isCleared = 0;

  for(; isCleared == 0; lower_bound+=2) {
    id_search();
    printf("asd%d\n", table->getChangedNum());
  }
  printf("D%d\n", table->getMD());

  table->dispData();

  return table->getStringSortData();
}
