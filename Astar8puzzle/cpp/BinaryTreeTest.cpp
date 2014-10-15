#include <iostream>
#include "BinaryTree.hpp"


int test_BinaryTree(){
 BinaryTree *test;
 int failed_test=0;

 // 挿入テスト
 test=new BinaryTree(5);
 failed_test+=(test->getData()==5)?0:1;
 
 // 追記
 for(int i=0;i<4;i++){
  test->append(2*i+1);
 }

 // 探索
 for(int i=1;i<8;i++){
  bool failCheck;
  failCheck=test->find(i);
  if(i%2==0){
   failCheck=!failCheck;
  }
  failed_test+=(failCheck)?0:1;
 }

 delete test;

 return failed_test;
}
