// 要素が1つ以上の時の二分木
// できること: 追加(ソレ以外全削除のほか必要ない)
// データ重複不可(する必要ない)
class BinaryTree{
 private:
  int data;
 public:
  // 小さい
  BinaryTree *left;
  // 大きい
  BinaryTree *right;
  BinaryTree(int data);
  bool find(int data);
  int getData();
  void append(int data);
};
