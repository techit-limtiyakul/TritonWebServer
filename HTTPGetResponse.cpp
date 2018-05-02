#include "HTTPGetResponse.hpp"

const string CLRF = "\r\n";

HTTPGetResponse::HTTPGetResponse(map<string, string> headers, string path) {
    this->headers = headers;
    this->absolutePath = path;
}

HTTPGetResponse::HTTPGetResponse() {
    headers = map<string, string>();
    absolutePath = "";
}

string HTTPGetResponse::getHeader(string key) const{
    auto pos = headers.find(key);
    if(pos == headers.end()){
        return "";
    }else{
        return pos->second;
    }
}

string HTTPGetResponse::getAbsolutePath() const{
    return std::string(absolutePath);
}

string HTTPGetResponse::toString() {
    string response = headers["version"] + " " + headers["code"] + CLRF;
    response += "Server:" + headers["server"] + CLRF;
    if(headers.count("Last-Modified") > 0)
    {
        response += "Last-Modified: " + headers["last_modified"] + CLRF;
    }
    if(headers.count("Content-Length") > 0)
    {
        response += "Content-Length:" + headers["content_length"] + CLRF;
    }
    response += "Content-Type:" + headers["content_type"] + CLRF;
    response += CLRF;

    return response;
}

