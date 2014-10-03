#ifndef __PROKON_CLIENT_H_INCLUDE__
#define __PROKON_CLIENT_H_INCLUDE__

#include <string>
#include <stdio.h>
#include <fstream>
#include <curl/curl.h>

using namespace std;

class ProkonClient{
 private:
  // streamにsize*nmembの大きさのptrを追加
  // 大きさを返す
  static size_t callbackWrite(char *ptr,size_t size, size_t nmemb,string *stream){
   // サイズ計算
   int dataLength = size * nmemb;
   // 書き込み
   stream->append(ptr,dataLength);
   // サイズを返す
   return dataLength;
  }
  string getData(string url,string postData);
  string getData(string url);
  // curlオブジェクト
  CURL *curl;
 public:
  // コンストラクタ
  ProkonClient();
  ~ProkonClient();
  static const string SERVER_ADDRESS;
  static const string TEAM_TOKEN;
  string getProblem(int problemNo);
  string sendAnswer(int problemNo,string answer);
};
#endif
