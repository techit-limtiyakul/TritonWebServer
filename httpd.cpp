#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
//#include <pthread.h>
#include "httpd.hpp"
#include <string.h>

#define MAXPENDING 15


struct ThreadArgs {
    int clntSock;
    string doc_root;
};


void *ThreadMain(void *threadArgs) {
    // Guarantees that thread resources are deallocated upon return
    pthread_detach(pthread_self());
    // Extract socket file descriptor from argument
    int clntSock = ((struct ThreadArgs *) threadArgs)->clntSock;
    string doc_root = ((struct ThreadArgs *) threadArgs)->doc_root;

    free(threadArgs); // Deallocate memory for argument
    HandleTCPClient(clntSock, doc_root);
    return (nullptr);
}


void start_httpd(unsigned short port, string doc_root) {

    cerr << "Starting server (port: " << port <<
         ", doc_root: " << doc_root << ")" << endl;

    int servSock;                    /* Socket descriptor for server */
    int clntSock;                    /* Socket descriptor for client */
    struct sockaddr_in servAddr; /* Local address */

    unsigned int clntLen;            /* Length of client address data structure */


    /* Construct local address structure */
    memset(&servAddr, 0, sizeof(servAddr));   /* Zero out structure */
    servAddr.sin_family = AF_INET;                /* Internet address family */
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    servAddr.sin_port = htons(port);            /* Local port */

    if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");

    if (::bind(servSock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
        DieWithError("bind() failed");

    /* Mark the socket so it will listen for incoming connections */
    if (listen(servSock, MAXPENDING) < 0)
        DieWithError("listen() failed");

    for (;;) {
        struct sockaddr_in clntAddr; /* Client address */
        /* Set the size of the in-out parameter */
        clntLen = sizeof(clntAddr);

        /* Wait for a client to connect */
        if ((clntSock = accept(servSock, (struct sockaddr *) &clntAddr, &clntLen)) < 0)
            DieWithError("accept() failed");

        // Set timeout for the client socket
        struct timeval timeout;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;

        if (setsockopt(clntSock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
        {
            DieWithError("setsockopt failed");
        }


        auto *threadArgs = new threadArgs();

        threadArgs->clntSock = clntSock;
        threadArgs->doc_root = doc_root;

        pthread_t threadID;
        int returnValue = pthread_create(&threadID, nullptr, ThreadMain, threadArgs);
        if(returnValue != 0){
            DieWithError("pthread_create() failed");
            printf("with thread %lu\n", (unsigned long int) threadID);
        }

        printf("Handling client %s socket %s\n", inet_ntoa(clntAddr.sin_addr), to_string(clntSock).c_str());

    }

}

void DieWithError(const char *errorMessage){
    printf("[ERROR] %s\n", errorMessage);
}
