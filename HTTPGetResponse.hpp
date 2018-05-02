
#ifndef P1_M_HTTPGETRESPONSE_H
#define P1_M_HTTPGETRESPONSE_H
#include <string>
#include <map>

using namespace std;

class HTTPGetResponse {
public:
    HTTPGetResponse(map<string, string> headers, string body);

    HTTPGetResponse();

    string getHeader(string key);

    string getAbsolutePath();

    string toString();

private:
    map<string, string> headers;
    string absolutePath;
};

#endif //P1_M_HTTPGETRESPONSE_H
