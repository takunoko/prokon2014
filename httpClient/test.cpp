#include <iostream>
#include <iomanip>
#include <algorithm> // for_each
#include "client.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;

int main(){
 ProkonClient client;
 string res;

#if DEBUG == 1
 cout << "Picture Downloading" << endl;
#endif
 try {
  // 引数に問題番号を指定して画像のバイナリstringを返す(cv::Mat形式にするかも)
  res=client.getProblem(1);
  
 } catch (runtime_error & exception) {

  cerr << "Exception: " << exception.what() << endl;
  exit(EXIT_FAILURE);
 }
 //cout << res << endl;
 vector<char> v(res.begin(),res.end());

#if 1
 cv::Mat pic;
 pic=cv::imdecode(cv::Mat(v),-1);
 cv::namedWindow("testWindow",CV_WINDOW_AUTOSIZE);
 cv::imshow("testWindow",pic);
 cv::waitKey(0);
#endif

 cout <<"Answer Send" << endl;

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
