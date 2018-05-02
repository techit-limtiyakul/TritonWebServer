#ifndef HTTPD_H
#define HTTPD_H

#include <string>
#include "HTTPGetRequest.hpp"
#include "HTTPGetResponse.hpp"

struct ThreadArgs;
void *ThreadMain(void *arg);
void start_httpd(unsigned short port, string doc_root);
void DieWithError(const char *errorMessage);  /* Error handling function */
void HandleTCPClient(int clntSocket, string doc_root);   /* TCP client handling function */
HTTPGetResponse PopulateResponse(const HTTPGetRequest &request, string doc_root);

#endif // HTTPD_H
