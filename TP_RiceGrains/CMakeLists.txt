PROJECT(TP1)
 
#Required in DGtal
CMAKE_MINIMUM_REQUIRED(VERSION 3.11)
set(CMAKE_CXX_STANDARD 11)

# Added by myself
set(CMAKE_PREFIX_PATH ~/libs)

FIND_PACKAGE(DGtal REQUIRED)
INCLUDE_DIRECTORIES(${DGTAL_INCLUDE_DIRS})
LINK_DIRECTORIES(${DGTAL_LIBRARY_DIRS})
 
ADD_EXECUTABLE(riceAnalysis main.cpp)
TARGET_LINK_LIBRARIES(riceAnalysis ${DGTAL_LIBRARIES})