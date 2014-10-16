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

// sort
#include "sort/PosData.h"
#include "sort/Process5.h"

// デバッグ用
#define VERBOSE

using namespace std;

// 問題解答関数
int solveProbrem(int id);

int main(int argc,char *argv[]){
 vector<int> ids;
 int id;

 if(argc==1){
  cout << "$ prokon25 id1" << endl;
  exit(EXIT_SUCCESS);
 }

 for(int i=1;i<argc;i++){
  ids.push_back(atoi(argv[i]));
 }
 id=ids.at(0);

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
 // PosData
 PosData *data;
 Process5 *sort;

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
 cerr << "Split Y: " << header.splitY << endl;
 cerr << "Selectable Count: " << header.selectableCount << endl;
 cerr << "Select Rate: " << header.selectRate << endl;
 cerr << "Exchange Rate: " << header.exchangeRate << endl;
#endif

 // Matにするために受け取った文字列をvector<char>型に変換
 vector<char> v(res.begin(),res.end());
 recievedData=cv::imdecode(cv::Mat(v),-1);

 data=new PosData(header.splitX,header.splitY);
 sort=new Process5(header.splitX,header.splitY);

#ifdef VERBOSE
 cout << "placement" << endl;
#endif
 // ここに画像解析処理
 // どこかでPosData設定が必要
 img= new PPMFILE(recievedData,header.splitX,header.splitY);
 img->calc_cost();
 img->calc_cost_maru();
 img->calc_cost_all();
 img->get_left_top();
 img->disp_cost_list(COST_ALL);
 // newはplacement_posが生成されない
 img->placement();

 img->disp_placement();

 img->set_PosData(data);
 data->dispData();

#ifdef VERBOSE
 cout << "sort" << endl;
#endif
 // ここにソート処理
 sort->importData(*data);

#ifdef VERBOSE
 cout <<"(not sort done)Answer And Send" << endl;
#endif

 res="";

 try {
  // 引数に問題番号を指定して画像のバイナリstringを返す(cv::Mat形式にするかも)
  res=client.sendAnswer(id,sort->sort());
 } catch (char const * exception) {

  cerr << "Exception: " << exception << endl;
  exit(EXIT_FAILURE);
 }
 // ACCEPTED XXかERRORがかえってくる
 // http://www.procon.gr.jp/modules/smartfaq/category.php?categoryid=23

 int error_part = 0;
 if(res=="ERROR"){
  cout << "ERROR OCCURED" << endl;
 }else if(res.substr(0,8)=="ACCEPTED"){
  error_part = atoi(res.substr(8).c_str());
  cout << atoi(res.substr(8).c_str()) << endl;
  if(error_part > 0) {
	 // ナンバー入り画像
	 img->create_num_img();
	 img->disp_img(NUM_IMG);
	 // 回答画像
	 img->create_result_img();
	 img->disp_img(RESULT_IMG);
   string change_place;
   string d1, d2;
   int p1, p2;
   while(1) {
    cin >> change_place;
    if(change_place[0] == 'q') break;
    for(int i = 0; change_place[i] != '\0'; i++) {
      if((change_place[i] > '0' && change_place[i] < '9') || change_place[i] == ',');
      else continue;
    }
    int j;
    for(j = 0; change_place[j] != ','; j++);
    p1 = atoi(change_place.c_str());
    // j+1の位置から２つ目の番号が始まっててほしい
    // j+1の位置から文字列をint型に変えたい
    p2 = atoi(change_place.c_str()+j+1);
    data->swapData(p1, p2);
   }
  }
 }

 cout << res << endl;

 return EXIT_SUCCESS;

}
