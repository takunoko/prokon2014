#ifndef __QUESTION_HEADER_H_INCLUDED__
#define __QUESTION_HEADER_H_INCLUDED__
// 問題ヘッダクラス
// ヘッダは分割数 選択可能回数 コスト変換レートが書かれている
// 画像側は分割数, ソート側は選択可能回数を使えばいいと思う
class QuestionHeader {
 public:
  // 分割数X,Y
  unsigned int splitX;
  unsigned int splitY;
  // レート(選択, 交換)
  unsigned int selectRate;
  unsigned int exchangeRate;
  // 交換可能回数
  unsigned int selectableCount;
};

#endif
