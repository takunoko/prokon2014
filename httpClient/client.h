#ifndef __PROKON_CLIENT_H_INCLUDE__
#define __PROKON_CLIENT_H_INCLUDE__

#include <string>

using namespace std;

class ProkonClient{
 private:
  static size_t callbackWrite(char *ptr,size_t size, size_t nmemb,string *stream){
   int dataLength = size * nmemb;
   stream->append(ptr,dataLength);
   return dataLength;
  }
 public:
  string getProbrem(int porbremNo);
};
#endif
