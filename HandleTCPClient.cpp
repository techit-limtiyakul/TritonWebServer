#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <unistd.h>     /* for close() */
#include <iostream>
#include <assert.h>
#include <fcntl.h>
#include "httpd.hpp"
#include "RequestFramer.hpp"
#include "RequestParser.hpp"

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

        framer.append(string(readBuffer, readBuffer+numBytesRcvd));
        if(framer.hasMessage()){
            string message = framer.topMessage();
            framer.popMessage();
            HTTPGetRequest request = parser.parse(message);

//            auto myMap = request.getHeaders();
//
//            for(auto it = myMap.cbegin(); it != myMap.cend(); ++it)
//            {
//                cout << it->first << " " << it->second << endl;
//            }

            HTTPGetResponse response = PopulateResponse(request, doc_root);

            string responseString = response.toString();
            send(clntSocket, responseString.c_str(), responseString.size() , 0);

            string path = response.getAbsolutePath();
            cout << "Path to file" << path << endl;
            if (path != "") {
                off_t fileOffset = 0;
                int fd = open(path.c_str(), O_RDONLY);
//                sendfile(clntSocket, fd, &fileOffset, BUFSIZE); for linux
                struct sf_hdtr* s;
                sendfile(clntSocket, fd, fileOffset, &fileOffset, s, 0);
            }

            if(request.getHeader("connection") == "closed")
                break;

        }

        numBytesRcvd = recv(clntSocket, readBuffer, BUFSIZE, 0);
        if (numBytesRcvd < 0)
            DieWithError("recv() failed");
    }
    
    close(clntSocket);    /* Close client socket */
}
