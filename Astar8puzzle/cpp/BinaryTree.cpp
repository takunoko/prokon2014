#include "BinaryTree.hpp"
// for NULL
#include <iostream>

BinaryTree::BinaryTree(int data){
 this->data=data;
 left=NULL;
 right=NULL;
}

bool BinaryTree::find(int data){
 if(this->data==data){
  return true;
 }else if(this->data > data){
  if(left==NULL){
   return false;
  }else{
   return left->find(data);
  }
 }else{
  if(right==NULL){
   return false;
  }else{
   return right->find(data);
  }
 }
}

int BinaryTree::getData(){
 return this->data;
}

void BinaryTree::append(int data){
 if(this->data > data){
  if(this->left==NULL){
   this->left = new BinaryTree(data);
  }else{
  }
 }
}
