#ifndef P1_M_RESPONSEBUILDER_HPP
#define P1_M_RESPONSEBUILDER_HPP
#include "HTTPGetRequest.hpp"
#include "HTTPGetResponse.hpp"

class ResponseBuilder {
public:
    HTTPGetResponse PopulateResponse(const HTTPGetRequest &request, string doc_root);
    HTTPGetResponse Build400ErrorResponse();
    HTTPGetResponse Build403ErrorResponse();
    HTTPGetResponse Build404ErrorResponse();
private:
    HTTPGetResponse BuildErrorResponse(const string errorCode);
};


#endif //P1_M_RESPONSEBUILDER_HPP
