#include <iostream>
#include "ResponseBuilder.hpp"
#include <sys/stat.h>
#include <ctime>
#include <chrono>

using namespace std;

const string VERSION = "HTTP/1.1";
const string SERVER = "Test Server";
const string jpg_type = ".jpg";
const string png_type = ".png";
const string html_type = ".html";

HTTPGetResponse ResponseBuilder::BuildErrorResponse(const string errorCode){
    map<string, string> headers;

    headers["version"] = VERSION;
    headers["server"] = SERVER;
    headers["code"] = errorCode;
    headers["content_type"] = "text/html";

    return HTTPGetResponse(headers);
}

HTTPGetResponse ResponseBuilder::Build400ErrorResponse() {
    return BuildErrorResponse("400 Client Error");
}

HTTPGetResponse ResponseBuilder::Build403ErrorResponse() {
    return BuildErrorResponse("403 Forbidden");

}

HTTPGetResponse ResponseBuilder::Build404ErrorResponse() {
    return BuildErrorResponse("404 Not Found");
}

HTTPGetResponse ResponseBuilder::PopulateResponse(const HTTPGetRequest &request, string doc_root){
    map<string, string> headers;

    headers["version"] = VERSION;
    headers["server"] = SERVER;
    headers["code"] = "200 OK";

    string filePath = request.getDirectory();
    if(filePath == "/")
        filePath = "/index.html";

    if(filePath.substr(0,1) != "/"){
        return Build400ErrorResponse();
    }

    filePath = doc_root + filePath;

    char resolved_path[200];
    char resolved_doc_root[200];
    realpath(filePath.c_str(), resolved_path);
    realpath(doc_root.c_str(), resolved_doc_root);

    string absolutePath = string(resolved_path);
    string absoluteDocRoot = string(resolved_doc_root);

    cout << absolutePath << endl;

    size_t idx = absolutePath.find_last_of('.');

    string extension = "";
    if(idx != string::npos){
        extension = absolutePath.substr(idx);
    }

    struct stat stat_buf;
    int rc = stat(absolutePath.c_str(), &stat_buf);

    if(request.getHeader("Host") == ""){
        return Build400ErrorResponse();
    }else if(rc == -1){
        return Build404ErrorResponse();
    }else if(!(stat_buf.st_mode & S_IROTH)){
        return Build403ErrorResponse();
    }else if(absolutePath.find(absoluteDocRoot) != 0){
        cout << absolutePath << "::" << absoluteDocRoot << endl;
        return Build404ErrorResponse();
    }

    if(extension == html_type){
        headers["content_type"] = "text/html";
    }else if(extension == jpg_type){
        headers["content_type"] = "image/jpeg";
    }else if(extension == png_type){
        headers["content_type"] = "image/png";
    }else{
        return Build400ErrorResponse();
    }

    headers["content_length"] = to_string(stat_buf.st_size);

//    auto now = std::chrono::system_clock::now();
//    std::time_t current_time = std::chrono::system_clock::to_time_t(now);
//    string timeString = std::ctime(&current_time);

    headers["last_modified"] = "Wed, 2 May 04:28:23 2018";

    return HTTPGetResponse(headers, absolutePath);
}