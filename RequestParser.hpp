#ifndef REQUESTPARSER_HPP
#define REQUESTPARSER_HPP

#include <string>
#include <stdint.h>
#include <map>
#include "HTTPGetRequest.hpp"

using namespace std;


class RequestParser {
public:
    static HTTPGetRequest parse(string request);

    static string parseFirstLine(const string &line);

    static pair<string, string> parseHeader(const string &line);
};

#endif // REQUESTPARSER_HPP
