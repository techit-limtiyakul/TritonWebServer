#ifndef HTTPD_H
#define HTTPD_H

#include <string>
#include "HTTPGetRequest.hpp"
#include "HTTPGetResponse.hpp"

using namespace std;

struct ThreadArgs;
void *ThreadMain(void *arg);
void start_httpd(unsigned short port, string doc_root);
void DieWithError(const char *errorMessage);  /* Error handling function */
void HandleTCPClient(int clntSocket);   /* TCP client handling function */
HTTPGetResponse PopulateResponse(const HTTPGetRequest &request);

#endif // HTTPD_H
