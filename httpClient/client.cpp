#include <curl/curl.h>
#include <iostream>
#include <sstream>
#include <string>

#include "client.h"

#define debug 1

using namespace std;

// サーバー
const string ProkonClient::SERVER_ADDRESS="localhost/web2/pic";
// チーム固有トークン
const string ProkonClient::TEAM_TOKEN="SKYHIGH\nCHRONOS\nENDLESS\n";

string ProkonClient::getProblem(int problemNo){
 // curlオブジェクト
 CURL *curl;
 // 実行結果
 CURLcode res;
 // httpコード (200とか404とか)
 long http_code=0;
 // ダウンロードしたデータ
 string chunk;


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
 curl = curl_easy_init();
 //curl=NULL;
 if(curl==NULL){
  cerr << "!!!curl_easy_init failed!!!" << endl;
  throw "CURL_FAILED";
 }

 curl_easy_setopt(curl,CURLOPT_URL,url.c_str());
 // port:8080
 curl_easy_setopt(curl,CURLOPT_PORT,8080);

 curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,ProkonClient::callbackWrite);
 curl_easy_setopt(curl,CURLOPT_WRITEDATA,&chunk);
 res=curl_easy_perform(curl);
 curl_easy_getinfo(curl,CURLINFO_RESPONSE_CODE,&http_code);
 curl_easy_cleanup(curl);

 if(res != CURLE_OK){
  cerr << "!!!curl_easy_perform failed!!!" << endl;
  throw "PERFORM FAILED";
 }
 if(http_code != 200){
  cerr << "STATUS CODE IS NOT 200:" << to_string(http_code) << endl;
  throw "STATUS CODE WRONG";
 }
 return chunk;
}

string ProkonClient::sendAnswer(int problemNo,string answer){
 CURL *curl;
 CURLcode res;
 long http_code=0;
 string data;
 string tmpStr;
 char *tmp;

 //http://{ServerAddress}/SubmitAnswer
 string url="http://" + SERVER_ADDRESS + "/SubmitAnswer.pl";
 string chunk;

 curl = curl_easy_init();
 //curl=NULL;
 if(curl==NULL){
  cerr << "!!!curl_easy_init failed!!!" << endl;
  throw "CURL_FAILED";
 }

 data="playerid=";
 tmp=curl_easy_escape(curl,TEAM_TOKEN.c_str(),TEAM_TOKEN.length());
 data+=tmp;
 curl_free(tmp);
 tmpStr=to_string(problemNo);
 tmp=curl_easy_escape(curl,tmpStr.c_str(),tmpStr.length());
 tmpStr=tmp;
 data+="&problemid=" + tmpStr;
 curl_free(tmp);
 tmp=curl_easy_escape(curl,answer.c_str(),answer.length());
 tmpStr=tmp;
 data+="&answer="+tmpStr;
 curl_free(tmp);


 // 要編集
 curl_easy_setopt(curl,CURLOPT_URL,url.c_str());
 // port:8080
 curl_easy_setopt(curl,CURLOPT_PORT,8080);
 // POST指定
 curl_easy_setopt(curl,CURLOPT_POSTFIELDS,data.c_str());

 curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,ProkonClient::callbackWrite);
 curl_easy_setopt(curl,CURLOPT_WRITEDATA,&chunk);
 res=curl_easy_perform(curl);
 curl_easy_getinfo(curl,CURLINFO_RESPONSE_CODE,&http_code);
 curl_easy_cleanup(curl);

 if(res != CURLE_OK){
  cerr << "!!!curl_easy_perform failed!!!" << endl;
  throw "PERFORM FAILED";
 }
 if(http_code != 200){
  cerr << "STATUS CODE IS NOT 200:" << to_string(http_code) << endl;
  throw "STATUS CODE WRONG";
 }
 return chunk;
}
