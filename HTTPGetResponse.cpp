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

string HTTPGetResponse::getHeader(string key) {
    return headers[key];
}

string HTTPGetResponse::getAbsolutePath() {
    return std::string(absolutePath);
}

string HTTPGetResponse::toString() {
    string response = headers["version"] + " " + headers["code"] + " " + headers["description"] + CLRF;
    response += "Server: " + headers["server"] + CLRF;
    if(headers["code"] == "200"){
        response += "Last-Modified: " + headers["last_modified"] + CLRF;
    }
    response += "Content-Type: " + headers["content_type"] + CLRF;
    response += "Content-Length " + headers["content_length"] + CLRF;

    return response;
}

