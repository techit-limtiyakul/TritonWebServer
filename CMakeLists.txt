cmake_minimum_required(VERSION 3.9)
project(p1_m)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(p1_m
        httpd.cpp
        httpd.hpp
        main.cpp
        RequestFramer.cpp
        RequestFramer.hpp
        RequestParser.cpp
        RequestParser.hpp
        HTTPGetRequest.cpp
        HTTPGetRequest.hpp
        HTTPGetResponse.hpp
        HTTPGetResponse.cpp
        HandleTCPClient.cpp
        PopulateResponse.cpp)