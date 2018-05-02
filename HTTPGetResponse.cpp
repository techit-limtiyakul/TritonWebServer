#include "HTTPGetResponse.hpp"

const string CRLF = "\r\n";

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
    string response = headers["version"] + " " + headers["code"] + CRLF;
    response += "Server:" + headers["server"] + CRLF;
    if(headers.count("content_length") > 0) {
        response += "Content-Length:" + headers["content_length"] + CRLF;
    }
    response += "Content-Type:" + headers["content_type"] + CRLF;
    if(headers.count("last_modified") > 0) {
        response += "Last-Modified:" + headers["last_modified"] + CRLF;
    }
    response += CRLF;

    return response;
}

