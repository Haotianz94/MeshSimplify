CMAKE_MINIMUM_REQUIRED( VERSION 2.8.7 )

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g -O3")
PROJECT(mesh_simplify)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${mesh_simplify_SOURCE_DIR}/bin)

SET(PROJECT_HEADER
        ${PROJECT_SOURCE_DIR}/include/common.h
        ${PROJECT_SOURCE_DIR}/include/heap.h
        ${PROJECT_SOURCE_DIR}/include/object.h)

SET(PROJECT_SRC
        ${PROJECT_SOURCE_DIR}/src/common.cpp
        ${PROJECT_SOURCE_DIR}/src/heap.cpp
        ${PROJECT_SOURCE_DIR}/src/object.cpp
        ${PROJECT_SOURCE_DIR}/src/main.cpp)

INCLUDE_DIRECTORIES(include)

ADD_EXECUTABLE(mesh_simplify ${PROJECT_HEADER} ${PROJECT_SRC})