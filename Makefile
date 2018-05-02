CC=g++
CFLAGS=-ggdb -std=c++11 -Wall -Wextra
DEPS = httpd.hpp RequestFramer.hpp RequestParser.hpp HTTPGetRequest.hpp HTTPGetResponse.hpp ResponseBuilder.hpp
SRCS = httpd.cpp RequestFramer.cpp RequestParser.cpp HTTPGetRequest.cpp HTTPGetResponse.cpp HandleTCPClient.cpp ResponseBuilder.cpp
MAIN_SRCS = main.cpp $(SRCS)
MAIN_OBJS = $(MAIN_SRCS:.c=.o)

default: httpd

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

httpd:    $(MAIN_OBJS)
	$(CC) $(CFLAGS) -o httpd $(MAIN_OBJS) -lpthread

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f httpd *.o
