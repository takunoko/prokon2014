#include <iostream>
#include "PosData.h"

using namespace std;

void PosData_hash_test();

int main(){
 PosData_hash_test();
 return 0;
}

void PosData_hash_test(){
 int data[9]={1,8,0,4,3,2,5,7,6};
 PosData *posDataObj=new PosData(3,3);
 int hash;

 for(int i=0;i<9;i++){
  posDataObj->setData(i%3,i/3,data[i]%3,data[i]/3);
 }

 hash=posDataObj->hash();
 posDataObj->dispData();
 cout << "hash: " << to_string(hash) << endl;;
 delete posDataObj;

 posDataObj=new PosData(3,3);
 posDataObj->dehash(hash);
 posDataObj->dispData();
 delete posDataObj;

 posDataObj=new PosData(4,2);
 for(int i=0;i<4*2;i++){
  posDataObj->setData(i%4,i/4,i%4,i/4);
 }
 hash=posDataObj->hash();
 posDataObj->dispData();
 cout << "hash: " << to_string(hash) << endl;;
 delete posDataObj;

 posDataObj=new PosData(4,2);
 posDataObj->dehash(hash);
 posDataObj->dispData();
 delete posDataObj;
}
