PROJECT(TP2)

#Required in DGtal
CMAKE_MINIMUM_REQUIRED(VERSION 3.11)
set(CMAKE_CXX_STANDARD 11)

FIND_PACKAGE(DGtal REQUIRED)
INCLUDE_DIRECTORIES(${DGTAL_INCLUDE_DIRS})
LINK_DIRECTORIES(${DGTAL_LIBRARY_DIRS})

ADD_EXECUTABLE(invariants main.cpp)
TARGET_LINK_LIBRARIES(invariants ${DGTAL_LIBRARIES})