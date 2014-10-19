#include <iostream>
#include <vector>
// for OpenCV2
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "setting.h"

// httpクライアント
#include "httpClient/client.h"
// 問題ヘッダ部(コメント部分)
#include "httpClient/QuestionHeader.hpp"

// 画像解析
#include "placement/PPMFILE.hpp"

// sort
#include "sort/PosData.h"
#include "sort/Process5.h"
#include "sort/Process6.h"
#include "sort/Process8.h"

// デバッグ用
//#define VERBOSE


//

using namespace std;

// 問題解答関数
int solveProbrem(int id);
// 手動回答
void swapPictureManual(PPMFILE *ppmData);

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
 // 5 or 6?
 Process5 *sort;
 //Process6 *sort6;

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
 cout << "Split X: " << header.splitX << endl
 << "Split Y: " << header.splitY << endl
 << "Selectable Count: " << header.selectableCount << endl
 << "Select Rate: " << header.selectRate << endl
 << "Exchange Rate: " << header.exchangeRate << endl;
#endif

 // Matにするために受け取った文字列をvector<char>型に変換
 vector<char> v(res.begin(),res.end());
 recievedData=cv::imdecode(cv::Mat(v),-1);

 data=new PosData(header.splitX,header.splitY);
 // TODO: Process5 or 6?
 sort=new Process5(header.splitX,header.splitY);

#ifdef VERBOSE
 cout << "placement" << endl;
#endif
 // ここに画像解析処理
 // どこかでPosData設定が必要
 img= new PPMFILE(recievedData,header.splitX,header.splitY);

 img->calc_cost_all();
 // newはplacement_posが生成されない
#ifdef WITH_PLACEMENT_5
 img->placement_5();
#elif defined(WITH_PLACEMENT_6)
 img->placement_6();
#else
 img->placement_4();
#endif
 // 画像作成
 img->create_result_img();
 if(img->chk_result() == true){
  cout << "result img GOOD" << endl;
 }else{
  cout << "result img BAD" << endl;
 }
 img->fix_pic_to_square();
 if(img->chk_result() == true){
  cout << "result img GOOD" << endl;
 }else{
  cout << "result img BAD" << endl;
 }
 // img->disp_img(RESULT_IMG);
 //img->disp_placement();
 //cv::waitKey(0);
#ifndef FIRST_AUTO_SUBMIT
 swapPictureManual(img);
#endif

 // placement後のデータをどうにかしないとヤバイ
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
  string sortResult=sort->sort();
  // \nを\x0d\x0aに変換
  string::size_type Pos=sortResult.find('\n');
  while(Pos!=string::npos){
   sortResult.replace(Pos,1,"\x0d\x0a");
   Pos=sortResult.find('\n',Pos+2);
  }
  res=client.sendAnswer(id,sortResult);
  cout << sortResult << endl;
 } catch (char const * exception) {

  cerr << "Exception: " << exception << endl;
  exit(EXIT_FAILURE);
 }
 // ACCEPTED XXかERRORがかえってくる
 // http://www.procon.gr.jp/modules/smartfaq/category.php?categoryid=23

 // もう一度: ここから(1回提出)
 bool flag=true;
 while(flag){
  // TODO:
  delete sort;
  //delete sort6;
  sort=new Process5(header.splitX,header.splitY);
  //sort6=new Process6(header.splitX,header.splitY);

  int error_part = 0;
  // 結果出力
  cout << res << endl;
  if(res=="ERROR"){
   cout << "ERROR OCCURED" << endl;
  }else if(res.substr(0,8)=="ACCEPTED"){
   error_part = atoi(res.substr(8).c_str());
   cout << atoi(res.substr(8).c_str()) << endl;
  }
  cout << "No2" << endl;
  // データ書き換え
  swapPictureManual(img);
  cout << "Setting" << endl;
  img->set_PosData(data);
  //data->dispData();
  cout << "importing" << endl;
  sort->importData(*data);
  cout << "sorting..." << endl;

  string sortResult=sort->sort();
  cout <<"Sort 5 complete" << endl;
  //string sortResult6=sort6->sort();
  //cout <<"Sort 6 complete" << endl;
  
  cout << "s/\\n/\\r\\n" << endl;
  string::size_type Pos=sortResult.find('\n');
  while(Pos!=string::npos){
   sortResult.replace(Pos,1,"\x0d\x0a");
   Pos=sortResult.find('\n',Pos+2);
  }
  cout << "sending..." << endl;

  res=client.sendAnswer(id,sortResult);
  cout << sortResult << endl;
 }

 return EXIT_SUCCESS;

}

// 手動入れ替え
void swapPictureManual(PPMFILE *ppmData){
 const string wrongPieceWindow="wrongPieceWindow";

 // 自動修正
 ppmData->fix_pic_to_square();


 // 画像表示
 ppmData->fix_manual(wrongPieceWindow);
 // 間違っているであろうパーツのウィンドウ
}
