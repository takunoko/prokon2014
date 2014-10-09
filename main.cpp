#include <iostream>
#include <vector>
// for OpenCV2
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

// httpクライアント
#include "httpClient/client.h"
// 問題ヘッダ部(コメント部分)
#include "httpClient/QuestionHeader.hpp"

// 画像解析
#include "placement/PPMFILE.hpp"

// デバッグ用
#define VERBOSE

using namespace std;

// 問題解答関数
int solveProbrem(int id);

int main(){
 int id=1;

 // マルチプロセスとかしたい時のために
 solveProbrem(id);

 return EXIT_SUCCESS;
}

int solveProbrem(int id){
 ProkonClient client; // httpクライアント
 QuestionHeader header; // ヘッダデータ
 string res; // 受信データ
 string answer; // 解答データ
 // 受信した画像データ
 cv::Mat recievedData;
 // PPMFILE
 PPMFILE *img;

#ifdef VERBOSE
 cout << "Picture Downloading\nID: " << id<< endl;
#endif

 try {
  // 引数に問題番号を指定して画像のバイナリstringを返す(cv::Mat形式にするかも)
  res=client.getProblem(id,header);
 } catch (runtime_error & exception) {
  cerr << "Exception: " << exception.what() << endl;
  exit(EXIT_FAILURE);
 }

#ifdef VERBOSE
 cerr << "Split X: " << header.splitX << endl;
 cerr << "Split Y: " << header.splitX << endl;
 cerr << "Selectable Count: " << header.selectableCount << endl;
 cerr << "Select Rate: " << header.selectRate << endl;
 cerr << "Exchange Rate: " << header.exchangeRate << endl;
#endif

 // Matにするために受け取った文字列をvector<char>型に変換
 vector<char> v(res.begin(),res.end());
 recievedData=cv::imdecode(cv::Mat(v),-1);

#if 0
 // 受信データ確認用ウィンドウ
 cv::namedWindow("testWindow",CV_WINDOW_AUTOSIZE);
 cv::imshow("testWindow",pic);
 cv::waitKey(0);
#endif

#ifdef VERBOSE
 cout << "placement" << endl;
#endif
 // ここに画像解析処理
 img= new PPMFILE(recievedData,header.splitX,header.splitY);

#ifdef VERBOSE
 cout << "sort" << endl;
#endif
 // ここにソート処理

#ifdef VERBOSE
 cout <<"Answer Send" << endl;
#endif

 try {
  // 引数に問題番号を指定して画像のバイナリstringを返す(cv::Mat形式にするかも)
  res=client.sendAnswer(id,"1145148938101919889466364364");
 } catch (char const * exception) {

  cerr << "Exception: " << exception << endl;
  exit(EXIT_FAILURE);
 }
 cout << res << endl;

 return EXIT_SUCCESS;

}
