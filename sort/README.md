# sort

## source cords
　クラスだけのファイル名の頭文字は大文字にしました。makeしたらオブジェクトファイルが隠しファイルとして生成される。

|          |          |
|:---------|----------|
|sort      |メイン    |
|Pos       |ポジション|
|Dataset   |ソートするデータ|
|Process?  |ソートをする部分|
|util      |便利な関数|
  
## 使い方
### Dataset
1.selectData(x, y)で指定した座標を選択
2.swapSesected(direction)で選択したデータを指定した向きに動かす．
3.dispData()で現在のデータを，dispDistance()で元の場所までの距離を表示します．
4.dispCost()で選択回数と交換回数を表示します．
  
* 選択回数と交換回数は自動で計算されます．
* select()で選択した後swapSelected()を使用することで初めて選択回数は増えます．
