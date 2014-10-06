#include <iostream>
#include <iomanip>
#include <algorithm> // for_each
#include "client.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#define VERBOSE 1

using namespace std;

int main(){
 ProkonClient client;
 QuestionHeader data;
 string res;

#if VERBOSE == 1
 cout << "Picture Downloading" << endl;
#endif
 try {
  // 引数に問題番号を指定して画像のバイナリstringを返す(cv::Mat形式にするかも)
  res=client.getProblem(1,data);
  
 } catch (runtime_error & exception) {

  cerr << "Exception: " << exception.what() << endl;
  exit(EXIT_FAILURE);
 }
 vector<char> v(res.begin(),res.end());

#if VERBOSE==1
 cerr << "Split X: " << data.splitX << endl;
 cerr << "Split Y: " << data.splitX << endl;
 cerr << "Selectable Count: " << data.selectableCount << endl;
 cerr << "Select Rate: " << data.selectRate << endl;
 cerr << "Exchange Rate: " << data.exchangeRate << endl;
#endif

#if 0
 cv::Mat pic;
 pic=cv::imdecode(cv::Mat(v),-1);
 cv::namedWindow("testWindow",CV_WINDOW_AUTOSIZE);
 cv::imshow("testWindow",pic);
 cv::waitKey(0);
#endif

#if VERBOSE == 1
 cout <<"Answer Send" << endl;
#endif

 try {
  // 引数に問題番号を指定して画像のバイナリstringを返す(cv::Mat形式にするかも)
  res=client.sendAnswer(1,"1145148938101919889466364364");
  
 } catch (char const * exception) {

  cerr << "Exception: " << exception << endl;
  exit(EXIT_FAILURE);
 }
 cout << res << endl;
 return 0;
}
