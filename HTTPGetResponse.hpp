
#ifndef P1_M_HTTPGETRESPONSE_H
#define P1_M_HTTPGETRESPONSE_H

#include <string>
#include <map>

using namespace std;

class HTTPGetResponse {
public:
    HTTPGetResponse(map<string, string> headers, string path = "");

    HTTPGetResponse();

    string getHeader(string key) const;

    string getAbsolutePath() const;

    string toString();

private:
    map<string, string> headers;
    string absolutePath;
};

#endif //P1_M_HTTPGETRESPONSE_H
