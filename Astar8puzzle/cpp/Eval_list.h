// 優先度付きキューのリスト

class Eval_list{
 private:
  // 状態
  int hash;
  // コレまでの経路(ハッシュ形式)
  vector<int> route;
  // 下2つはコスト計算に使う
  // どの階層か
  int floor;
  // どのくらい離れているか
  int evaluation;
 public:
  bool operator < (Eval_list &data){
   return (this->evaluation + this->floor) < (data->evaluation + data->floor);
  }
  bool operator > (Eval_list &data){
   return (this->evaluation + this->floor) > (data->evaluation + data->floor);
  }
}
