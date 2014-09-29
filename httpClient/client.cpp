#include <curl/curl.h>
#include <iostream>
#include <string>

#include "client.h"

using namespace std;

#define SERVER_ADDRESS 127.0.0.1

string ProkonClient::getProbrem(int porbremNo){
 CURL *curl;
 CURLcode res;
 string chunk;

 curl = curl_easy_init();
 if(curl==NULL){
  cerr << "!!!curl_easy_init failed!!!" << endl;
  throw "CURL_FAILED";
  return "";
 }

 // 要編集
 curl_easy_setopt(curl,CURLOPT_URL,"http://localhost/index.html");
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
