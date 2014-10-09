#include <iostream>
#include <iomanip>
#include <algorithm> // for_each
#include "client.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <unistd.h>

#define VERBOSE

using namespace std;

int solveProbrem(int id);
int main(){

 int id=1;
 int childProcess=0;
 // 問題が9だった
 //pid_t problemNo[9];
 
 for(int i=0;i<9;i++){
  id=i+1;
  pid_t pid;
  switch (pid=fork()) {
   case 0:
    solveProbrem(id);
    exit(EXIT_SUCCESS);
    break;
   case -1:
    cerr << "fork failed: " << id << endl << strerror(errno) << endl;
    break;
   default:
    childProcess++;
#ifdef VERBOSE
    cout << "forked id: " << id << "pid: " << pid << endl;
#endif
    break;
  }
 }

 while(childProcess>0){
  pid_t pid=wait(NULL);
  childProcess--;
#ifdef VERBOSE
  cout << "Complete: " << pid << endl;
#endif
 }

 return 0;
}

int solveProbrem(int id){
 ProkonClient client;
 QuestionHeader data;
 string res;

#ifdef VERBOSE
 cout << "Picture Downloading\nID: " << id<< endl;
#endif

 try {
  // 引数に問題番号を指定して画像のバイナリstringを返す(cv::Mat形式にするかも)
  res=client.getProblem(id,data);
 } catch (runtime_error & exception) {

  cerr << "Exception: " << exception.what() << endl;
  exit(EXIT_FAILURE);
 }
 vector<char> v(res.begin(),res.end());

#ifdef VERBOSE
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
