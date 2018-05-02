#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "httpd.hpp"

using namespace std;

//equivalent to old main
//call frame and parse logic
//multithread with new clients
//append file to docroot

#define MAXPENDING 5


struct ThreadArgs {
    int clntSock;
};

const int timeout = 5000;

void *ThreadMain(void *threadArgs) {
    // Guarantees that thread resources are deallocated upon return
    pthread_detach(pthread_self());
    // Extract socket file descriptor from argument
    int clntSock = ((struct ThreadArgs *) threadArgs)->clntSock;

    // Set timeout for the client socket
    if(setsockopt(clntSock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
        DieWithError("setsockopt() failed");

    free(threadArgs); // Deallocate memory for argument
    HandleTCPClient(clntSock);
    return (nullptr);
}

void start_httpd(unsigned short port, string doc_root) {

    cerr << "Starting server (port: " << port <<
         ", doc_root: " << doc_root << ")" << endl;

    int servSock;                    /* Socket descriptor for server */
    int clntSock;                    /* Socket descriptor for client */
    struct sockaddr_in echoServAddr; /* Local address */
    struct sockaddr_in echoClntAddr; /* Client address */
    unsigned short echoServPort;     /* Server port */
    unsigned int clntLen;            /* Length of client address data structure */

    if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");

    /* Construct local address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));   /* Zero out structure */
    echoServAddr.sin_family = AF_INET;                /* Internet address family */
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    echoServAddr.sin_port = htons(echoServPort);      /* Local port */

    /* Mark the socket so it will listen for incoming connections */
    if (listen(servSock, MAXPENDING) < 0)
        DieWithError("listen() failed");

    for (;;) {
        /* Set the size of the in-out parameter */
        clntLen = sizeof(echoClntAddr);

        /* Wait for a client to connect */
        if ((clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr,
                               &clntLen)) < 0)
            DieWithError("accept() failed");

        auto *threadArgs = (struct ThreadArgs *) malloc(sizeof(struct ThreadArgs));
        if (threadArgs == nullptr){
            DieWithError("malloc() failed");
        }
        threadArgs->clntSock = clntSock;
        pthread_t threadID;
        int returnValue = pthread_create(&threadID, nullptr, ThreadMain, threadArgs);
        if(returnValue != 0){
            DieWithError("pthread_create() failed");
            printf("with thread %lu\n", (unsigned long int) threadID);
        }

        printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));

    }

}

void DieWithError(const char *errorMessage){
    printf("[ERROR] %s\n", errorMessage);
}
