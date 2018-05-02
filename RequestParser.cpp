#include <assert.h>
#include "RequestParser.hpp"
#include <vector>
#include <regex>
#include <iostream>

using namespace std;

HTTPGetRequest RequestParser::parse(std::string request) {
    vector<string> requestLines;


    regex newlineRe("\\r\\n");

    sregex_token_iterator it(request.begin(), request.end(), newlineRe, -1);
    sregex_token_iterator reg_end;

    try {
        for (; it != reg_end; ++it) {
            string line = it->str();
            requestLines.push_back(line);
        }

        string firstLine = requestLines[0];
        string fileDirectory = parseFirstLine(firstLine);

        map<string, string> headers;
        //second to second-to-last lines are headers (last one is empty)
        for(unsigned i=1; i<requestLines.size()-1; i++)
        {
            headers.insert(parseHeader(requestLines[i]));
        }

        return HTTPGetRequest(fileDirectory, headers);
    }
    catch (std::exception &e){
        cout << e.what() << endl;
        throw e;
    }
}

string RequestParser::parseFirstLine(const string &line) {
    regex firstLineRe("^GET (\\S+) HTTP/1.1$");
    smatch matchedResult;

    if(regex_match(line, matchedResult, firstLineRe))
    {
         return matchedResult[1];
    }

    throw std::runtime_error("Invalid request");
}

std::pair<string,string> RequestParser::parseHeader(const string &line) {
    regex headerLineRe("^([^:\\s]+)\\s*:\\s*(.*)$");
    smatch matchedResult;

    if(regex_match(line, matchedResult, headerLineRe))
    {
        return std::make_pair(matchedResult[1], matchedResult[2]);
    }

    throw std::runtime_error("Invalid request");
}
