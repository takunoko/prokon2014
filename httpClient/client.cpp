#include <curl/curl.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
// for isdigit
#include <cctype>

#include "client.h"
#include "QuestionHeader.hpp"

#define DEBUG 1

using namespace std;

// サーバー
//const string ProkonClient::SERVER_ADDRESS="172.168.1.2";
const string ProkonClient::SERVER_ADDRESS="localhost/web2/pic/";
// チーム固有トークン
const string ProkonClient::TEAM_TOKEN="0768109858";
#define PORT 8080

// コンストラクタ
ProkonClient::ProkonClient(){
 curl = curl_easy_init();
 if(curl==NULL){
  throw runtime_error("CURL is NULL\n");
 }
}
// デストラクタ
ProkonClient::~ProkonClient(){
 curl_easy_cleanup(curl);
}

string ProkonClient::getProblem(int problemNo,QuestionHeader & data){
 // 受信データ
 string chunk;
 // 入れるポインタデータ
 vector<unsigned int *> dataPointer;
 // 現在操作するポインタ
 vector<unsigned int *>::iterator now;
 // フラグ
 bool enable;

 // 代入するデータを順に入れる
 dataPointer.push_back(& data.splitX);
 dataPointer.push_back(& data.splitY);
 dataPointer.push_back(& data.selectableCount);
 dataPointer.push_back(& data.selectRate);
 dataPointer.push_back(& data.exchangeRate);

 // URL用問題番号の文字列
 ostringstream probStr;
 //問題番号部分指定
 probStr.setf(ios::right);
 probStr.fill('0');
 probStr.width(2);
 probStr << problemNo;

 // URL
 string url="http://" + SERVER_ADDRESS + "/problem/prob" + probStr.str() + ".ppm";
#if DEBUG==1
 cout << url << endl;
#endif

 chunk=getData(url);
 // コメント抜き取り
 now=dataPointer.begin();
 **now=0;
 enable=false;
 for(char &c : chunk){
  // コメント読み取り有効
  if(c=='#'){
   // 有効
   enable=true;
  }
  // コメント読み取り無効
  if(c=='\n'){
   enable=false;
  }
  // コメント範囲内であれば
  if(enable){
#ifdef VERBOSE
   cerr << "c: " << c << endl << "enable:" << enable << endl;
#endif
   if(isdigit(c)){
    **now= (**now) * 10+(c-'0');
   }else if(**now!=0){
    now++;
    if(now == dataPointer.end()){
     break;
    }else{
     **now=0;
    }
   }
  }
 }
 return chunk;
}

string ProkonClient::sendAnswer(int problemNo,string answer){
 // 送信データ
 string data;
 // 受信データ
 string chunk;
 //http://{ServerAddress}/SubmitAnswer
 string url="http://" + SERVER_ADDRESS + "/SubmitAnswer.pl";
 // エスケープされた文字列 free用
 char * escapedStr;

 data="playerid=";
 // チームトークン
 escapedStr=curl_easy_escape(curl,TEAM_TOKEN.c_str(),0);
 data+=escapedStr;
 curl_free(escapedStr);
 // 問題番号
 escapedStr=curl_easy_escape(curl,to_string(problemNo).c_str(),0);
 data+="&problemid="+(string)(escapedStr);
 curl_free(escapedStr);
 //回答
 escapedStr=curl_easy_escape(curl,answer.c_str(),0);
 data+="&answer="+(string)escapedStr;
 curl_free(escapedStr);

 chunk=getData(url,data);

 return chunk;
}

// postData: *直接送信する* (エスケープが必要)
string ProkonClient::getData(string url,string postData){
 CURLcode res;
 // httpコード (200とか404とか)
 long http_code=0;
 // ダウンロードしたデータ
 string chunk;

 // URL設定
 curl_easy_setopt(curl,CURLOPT_URL,url.c_str());
 // port:8080
 curl_easy_setopt(curl,CURLOPT_PORT,PORT);
 // POSTデータ指定
 curl_easy_setopt(curl,CURLOPT_POSTFIELDS,postData.c_str());

 // 受信コールバック
 curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,ProkonClient::callbackWrite);
 // コールバックデータ書き込み先
 curl_easy_setopt(curl,CURLOPT_WRITEDATA,&chunk);
 // 実行
 res=curl_easy_perform(curl);
 // レスポンスコード受信
 curl_easy_getinfo(curl,CURLINFO_RESPONSE_CODE,&http_code);

 if(res != CURLE_OK){
  cerr << "!!!curl_easy_perform failed!!!" << endl;
  throw runtime_error("perform something wrong.");
 }
 if(http_code != 200){
  cerr << "STATUS CODE IS NOT 200:" << to_string(http_code) << endl;
  string msg="http code is "+to_string(http_code);
  throw runtime_error(msg);
 }
 return chunk;
}

string ProkonClient::getData(string url){
 return getData(url,"");
}

size_t ProkonClient::callbackWrite(char *ptr,size_t size, size_t nmemb,string *stream){
 // サイズ計算
 int dataLength = size * nmemb;
 // 書き込み
 stream->append(ptr,dataLength);
 // サイズを返す
 return dataLength;
}
