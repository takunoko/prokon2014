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
    if(!checkPosEqual(Pos(i%table->getWidth(), i/table->getWidth()), table->selected)) {
      for(j = i; j < table->getWidth()*table->getHeight(); j++) {
        data = table->getData(Pos(j%table->getWidth(), j/table->getWidth()));
        if(data2.x+data2.y*table->getWidth() > (data.x+data.y*table->getWidth())) {
          num++;
        }
      }
    }
    sum += num;
  }
  return sum % 2;
}


int Pro8::id_search() {
  //printf("D%d,%d\n", table->getMD(), lower_bound);
  //printf("S%d,%d\n", table->getChangedNum(), move);
    //printf("%d\n", table->getMD());
    //table->dispData();
  if(move >= limit) {
    if(table->getMD() <= start_bound) {
      isCleared = 1;
      return 1;
    }
  } else {
    for(int k = 0; k < 8; k+=2) {
      Pos p = surroundings(table->selected, k);
      if(!checkInScope(table->getWidth(), table->getHeight(), p.x, p.y)) {
        continue;
      }
      if(getReversedDirection(table->getLastMove()) == k) {
        continue;
      }
      if(table->swapSelected(k)) {
        continue;
      }
      if(move+table->getMD() <= limit) {
        move++;
        int is = id_search();
        move--;
        if(is) {
          return 1;
        }
      }
      table->undo();
    }
  }
  return 0;
}

void Pro8::importData(PosData &data) {
  table->importData(data);
}

string Pro8::sort() {
  table->findAndSelectData(Pos(table->getWidth()-1, table->getHeight()-1));
  printf("parity = %d\n", calcParity());
  if(calcParity() != 0) {
    table->dispOne();
    puts("ppppppp");
    printf("panapp %d, %d\n", table->getWidth()-2, table->getHeight()-1);
    Pos dummy2 = Pos(table->getWidth()-1, table->getHeight()-1);
    if(checkPosEqual((dummy2), table->getData(Pos(table->getWidth()-2, table->getHeight()-1))) ) {
      table->selectData(Pos(table->getWidth()-2, table->getHeight()-2));
    } else
      table->selectData(Pos(table->getWidth()-2, table->getHeight()-1));
    Pos dummy = surroundings(table->getSelected(), RIGHT);
    if(checkPosEqual((dummy), table->getData(dummy2)) ) {
      table->swapSelected(LEFT);
    } else 
      table->swapSelected(RIGHT);
  }
  table->findAndSelectData(Pos(table->getWidth()-1, table->getHeight()-1));
  table->dispData();
  table->dispOne();
  table->calcMD();
  printf("---%d\n", table->getMD());
  printf("parity = %d\n", calcParity());
  start_bound = calcParity();

  lower_bound = table->getMD();
  limit = table->getMD();
  start_bound = calcParity();
  printf("D%d\n", table->getMD());
  isCleared = 0;
  move = 0;

  for(; isCleared == 0 && limit < 82; limit+=2) {
    id_search();
    printf("asd%d\n", table->getChangedNum());
  }
  printf("D%d\n", table->getMD());

  table->dispData();

  return table->getStringSortData();
}
