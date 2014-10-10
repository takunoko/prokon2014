#ifndef __PROKON_CLIENT_H_INCLUDED__
#define __PROKON_CLIENT_H_INCLUDED__

#include <string>
#include <stdio.h>
#include <fstream>
#include <curl/curl.h>

#include "QuestionHeader.hpp"

using namespace std;

class ProkonClient{
 private:
  // streamにsize*nmembの大きさのptrを追加
  // 大きさを返す
  static size_t callbackWrite(char *ptr,size_t size, size_t nmemb,string *stream);
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
  string getProblem(int problemNo,QuestionHeader & data);
  string sendAnswer(int problemNo,string answer);
};
#endif
