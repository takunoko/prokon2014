// 3x3固定

#include <iostream>
#include <string>
#include "PosData.h"

using namespace std;

class Cells : public PosData{
 public:
  string toString();
  Cells();
  Cells(int hash);
  int hash();
};

Cells::Cells() : PosData(3,3){
  int data[9]={1,8,0,4,3,2,5,7,6};

  for(int i=0;i<9;i++){
   setData(i%3,i/3,data[i]%3,data[i]/3);
  }
}

Cells::Cells(int hash) : PosData(3,3){
 for(int i=0;i<9;i++){
  int data=hash%9;
  setData((8-i)%3, (8-i)/3,data%3,data/3);
  hash/=9;
 }
}

// サンプルとは違う方法でハッシュにする
// 1桁が取りうる値は0-8
// 9進数として考える
// もっと数値を小さくする方法があるかも
int Cells::hash(){
 int hash=0;
 for(int i=0;i<9;i++){
  hash=hash*9+getY(i%3,i/3)*3+getX(i%3,i/3);
 }
 return hash;
}

string Cells::toString(){
 string ret="";

 for(int i=0;i<9;i++){ 
  ret+="[" + to_string(getY(i%3,i/3)*3 + getX(i%3,i/3))+ "]";
  if((i+1)%3==0){
   ret+="\n";
  }
 }

 return ret;
}

int main(){
 Cells *test=new Cells;
 int hash;
 cout << test->toString() << endl;
 test->dispData();
 hash=test->hash();
 delete test;
 cout << hash << endl;
 test=new Cells(hash);
 cout << test->toString() << endl;
 test->dispData();
}
