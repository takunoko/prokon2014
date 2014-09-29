#include <curl/curl.h>
#include <iostream>
#include <fstream>
#include <string>

#include "client.h"

using namespace std;

const string ProkonClient::SERVER_ADDRESS="localhost";

string ProkonClient::getProbrem(int probremNo){
 CURL *curl;
 CURLcode res;
 string url="http://" + SERVER_ADDRESS + "/web2/pic/" + to_string(probremNo) + ".ppm";
 //string url="http://localhost/web2/pic/1.ppm";
 string chunk;

 curl = curl_easy_init();
 //curl=NULL;
 if(curl==NULL){
  cerr << "!!!curl_easy_init failed!!!" << endl;
  throw "CURL_FAILED";
 }

 // 要編集
 curl_easy_setopt(curl,CURLOPT_URL,url.c_str());
 // port:8080
 curl_easy_setopt(curl,CURLOPT_PORT,8080);

 curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,ProkonClient::callbackWrite);
 curl_easy_setopt(curl,CURLOPT_WRITEDATA,&chunk);
 res=curl_easy_perform(curl);
 curl_easy_cleanup(curl);

 if(res != CURLE_OK){
  cerr << "!!!curl_easy_perform failed!!!" << endl;
  throw "PERFORM FAILED";
 }
 return chunk;
}
