// 3x3固定

#include <iostream>
#include <string>

using namespace std;

class Cells{
 public:
  int cell[9];
  string toString();
  Cells();
  Cells(int hash);
  int hash();
};

Cells::Cells(){
  cell[ 0 ]=1;
  cell[ 1 ]=8;
  cell[ 2 ]=0;
  cell[ 3 ]=4;
  cell[ 4 ]=3;
  cell[ 5 ]=2;
  cell[ 6 ]=5;
  cell[ 7 ]=7;
  cell[ 8 ]=6;
}

Cells::Cells(int hash){
 for(int i=0;i<9;i++){
  cell[8-i]=hash%9;
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
  hash=hash*9+cell[i];
 }
 return hash;
}

string Cells::toString(){
 string ret="";

 for(int i=0;i<9;i++){ 
  ret+="[" + to_string(cell[i]) + "]";
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
 hash=test->hash();
 delete test;
 cout << hash<< endl;
 test=new Cells(hash);
 cout << test->toString() << endl;
}
