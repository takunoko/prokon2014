// 優先度付きキューのリスト

class Eval_list{
 private:
  // 状態
  int hash;
  // コレまでの経路
  vector<int> route;
  // 下2つはコスト計算に使う
  // どの階層か
  int floor;
  // どのくらい離れているか
  int evaluation;
}
