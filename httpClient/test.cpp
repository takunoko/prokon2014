#include <iostream>
#include <iomanip>
#include <algorithm> // for_each
#include "client.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;

int main(){
 ProkonClient client;
 ofstream ofs("./downloaded.ppm");
 string res;
 try {
  res=client.getProbrem(1);
  
 } catch (char * exception) {

  cerr << "Exception: " << exception << endl;
  exit(EXIT_FAILURE);
 }
 vector<char> v(res.begin(),res.end());

 cv::Mat pic;
 pic=cv::imdecode(cv::Mat(v),-1);
 cv::namedWindow("testWindow",CV_WINDOW_AUTOSIZE);
 cv::imshow("testWindow",pic);
 cv::waitKey(0);
 return 0;
}
