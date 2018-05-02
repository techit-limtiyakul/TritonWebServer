#ifndef P1_M_HTTPGETREQUEST_HPP
#define P1_M_HTTPGETREQUEST_HPP

#include <string>
#include <map>

using namespace std;

class HTTPGetRequest {
public:
    HTTPGetRequest(string dir, map<string, string> allHeaders);

    HTTPGetRequest();

    string getDirectory() const;

    string getHeader(string key) const;

    map<string, string> getHeaders() const;

private:
    string fileDirectory;
    map<string, string> headers;
};


#endif //P1_M_HTTPGETREQUEST_HPP
