#include "HTTPGetRequest.hpp"

using namespace std;

HTTPGetRequest::HTTPGetRequest(std::string dir, map<string, string> allHeaders)
{
    fileDirectory = dir;
    headers = allHeaders;
}

HTTPGetRequest::HTTPGetRequest()
{
    fileDirectory = "";
    headers = map<string, string>();
}

string HTTPGetRequest::getDirectory(){
    return fileDirectory;
}

string HTTPGetRequest::getHeader(string key){
    return headers[key];
}
