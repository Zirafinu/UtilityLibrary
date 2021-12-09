cmake_minimum_required(VERSION 3.14)
include (FetchContent)

FetchContent_Declare(DocTest 
    GIT_REPOSITORY "https://github.com/onqtam/doctest"         
    GIT_TAG "932a2ca50666138256dae56fbb16db3b1cae133a"
    SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/doctest" )
FetchContent_Declare(GSL 
    GIT_REPOSITORY "https://github.com/microsoft/GSL/"         
    GIT_TAG "0f6dbc9e2915ef5c16830f3fa3565738de2a9230"
    SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/gsl" )
FetchContent_MakeAvailable(DocTest GSL)
