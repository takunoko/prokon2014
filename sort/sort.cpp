#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include "Dataset.h"
#include "Process1.h"
#include "Process2.h"
#include "Process3.h"
#include "Process4.h"
#include "Process5.h"
#define WIDTH 3
#define HEIGHT 3

int main(void) {
  PosData *data = new PosData(WIDTH, HEIGHT);
  string data_send;
  int i, j, x, y;
  int k; // for pointer

  srand((unsigned)time(NULL));

  // データ設定
  vector<int> origdata;
  for(i=0;i<9;i++){
   origdata.push_back(i);
  }

  k=0;
  while(origdata.size()!=0){
   j=rand()% origdata.size();
   i=origdata.at(j);
   origdata.erase(origdata.begin()+j);
   cout << "(" << k/HEIGHT << "," << k%HEIGHT << ")=(" << i/HEIGHT << "," << i%HEIGHT << ")"<< endl;
   data->setData(k/HEIGHT,k%HEIGHT,i/HEIGHT,i%HEIGHT);
   k++;
  }

  /*
  for(i = 0; i < data->getHeight(); i++) {
    for(j = 0; j < data->getWidth(); j++) {
      x = rand() % data->getWidth();
      y = rand() % data->getHeight();
      data->setData(i, j, i, j);
    }
  }

  */

  // ソート
  Process5 *test = new Process5(WIDTH, HEIGHT);
  test->importData(*data);
  data_send = test->sort();
  delete test;
  cerr << data_send << endl;

  return 0;
}

