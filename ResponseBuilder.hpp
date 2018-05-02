#ifndef P1_M_RESPONSEBUILDER_HPP
#define P1_M_RESPONSEBUILDER_HPP
#include "HTTPGetRequest.hpp"
#include "HTTPGetResponse.hpp"

class ResponseBuilder {
public:
    HTTPGetResponse BuildErrorResponse(const string errorCode);
    HTTPGetResponse PopulateResponse(const HTTPGetRequest &request, string doc_root);
};


#endif //P1_M_RESPONSEBUILDER_HPP
