#include <map>
#include "PosData.h"

// AstarでPosDataをとくためのアルゴリズム
class Astar{
 private:
  // 訪問済みリスト
  map<int,bool> visited;
  inline void setVisited(int hash){
   visited.insert(map<int,bool>::value_type(hash,true));
  }
  inline bool isVisited(int hash){
   return !(visited.find(hash)==visited.end());
  }
 public:
  Astar(PosData &data);
};
