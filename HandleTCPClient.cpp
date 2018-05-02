#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <unistd.h>     /* for close() */
#include <iostream>
#include <fcntl.h>
#include "httpd.hpp"
#include "RequestFramer.hpp"
#include "RequestParser.hpp"
#include "ResponseBuilder.hpp"
#include <sys/sendfile.h>

using namespace std;

size_t BUFSIZE = 512;
void HandleTCPClient(int clntSocket, const string doc_root)
{
    RequestFramer framer;
    RequestParser parser;
    char readBuffer[BUFSIZE];

    // Receive message from client
    ssize_t numBytesRcvd = recv(clntSocket, readBuffer, BUFSIZE, 0);

    if (numBytesRcvd < 0)
        DieWithError("recv() failed");

    // Send received string and receive again until end of stream
    while (numBytesRcvd > 0) { // 0 indicates end of stream

        bool isDone = false;
        framer.append(string(readBuffer, readBuffer+numBytesRcvd));

        while(framer.hasMessage() && !isDone){
            string message = framer.topMessage();
            framer.popMessage();

            HTTPGetResponse response;
            ResponseBuilder builder;
            HTTPGetRequest request = HTTPGetRequest();

            try{
                request = parser.parse(message);
                response = builder.PopulateResponse(request, doc_root);
            } catch(exception &e) {
                response = builder.Build400ErrorResponse();
            }

            string responseString = response.toString();
            send(clntSocket, responseString.c_str(), responseString.size() , 0);

            string path = response.getAbsolutePath();
            printf("Requesting file from: %s\n", path.c_str());

            if (path != "") {
                off_t fileOffset = 0;
                int fd = open(path.c_str(), O_RDONLY);
                //for linux
                int filesize = stoi(response.getHeader("content_length"));
                sendfile(clntSocket, fd, &fileOffset, filesize);
//                struct sf_hdtr* s;
//                sendfile(fd, clntSocket, fileOffset, &fileOffset, s, 0);
            }

            if(request.getHeader("Connection") == "close")
                isDone = true;

        }

        if(isDone)
            break;

        numBytesRcvd = recv(clntSocket, readBuffer, BUFSIZE, 0);
        if (numBytesRcvd < 0)
            DieWithError("recv() failed");
    }
    
    close(clntSocket);    /* Close client socket */
}
