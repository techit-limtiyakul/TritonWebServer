#include <iostream>
#include "ResponseBuilder.hpp"
#include <sys/stat.h>
#include "HTTPGetRequest.hpp"
#include "HTTPGetResponse.hpp"
#include <errno.h>

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

    return HTTPGetResponse(headers);
}

HTTPGetResponse ResponseBuilder::PopulateResponse(const HTTPGetRequest &request, string doc_root){
    map<string, string> headers;

    headers["version"] = VERSION;
    headers["server"] = SERVER;
    headers["code"] = "200 OK";

    string filePath = request.getDirectory();
    if(filePath == "/")
        filePath = "/index.html";
    else if(filePath.substr(0,1) != "/"){
        return BuildErrorResponse("400 Error");
    }

    size_t idx = filePath.find_last_of('.');
    string extension = filePath.substr(idx);

    filePath = doc_root + filePath;

    char resolved_path[200];
    realpath(filePath.c_str(), resolved_path);

    string absolutePath = string(resolved_path);

    cout << absolutePath << endl;

    struct stat stat_buf;
    int rc = stat(absolutePath.c_str(), &stat_buf);

    if(request.getHeader("Host") == ""){
        headers["code"] = "400 Client Error";
    }else if(extension != jpg_type && extension != png_type && extension != html_type) {
        headers["code"] = "400 Client Error";
    }else if(rc == -1){
        headers["code"] = "404 Not Found";
    }else if(!(stat_buf.st_mode & S_IROTH)){
        headers["code"] = "403 Forbidden";
    }

    if(headers["code"] != "200 OK" || extension == html_type){
        headers["content_type"] = "text/html";
    }else if(extension == jpg_type){
        headers["content_type"] = "image/jpg";
    }else{
        headers["content_type"] = "image/png";
    }

    if(headers["code"] == "200 OK"){
        headers["content_length"] = to_string(stat_buf.st_size);
        return HTTPGetResponse(headers, absolutePath);
    }

    printf("What have gone wrong here: %s\n", strerror(errno));

    return HTTPGetResponse(headers);
}