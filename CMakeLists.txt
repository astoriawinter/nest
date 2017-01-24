cmake_minimum_required(VERSION 3.6)
project(Nest)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c99")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${Nest_SOURCE_DIR}/cmake")

set(SDL2_PATH "C:\\SDL2\\x86_64-w64-mingw32")
find_package(SDL2 REQUIRED)
find_package(SDL2_Image REQUIRED)
find_package(SDL2_ttf REQUIRED)

set(CMAKE_PREFIX_PATH tmx/deps)

include_directories(${Nest_SOURCE_DIR}/tmx/deps/include/libxml2)
list(APPEND libs ${Nest_SOURCE_DIR}/tmx/deps/lib/libxml2.a)
list(APPEND libs ${Nest_SOURCE_DIR}/tmx/deps/lib/libxml2.dll.a)
list(APPEND libs -lws2_32)

find_package(ICONV REQUIRED)
include_directories(${ICONV_INCLUDE_DIR})
list(APPEND libs ${ICONV_LIBRARIES})

include_directories(${Nest_SOURCE_DIR}/tmx/deps/include/zlib)
list(APPEND libs ${Nest_SOURCE_DIR}/tmx/deps/lib/libz.a)
list(APPEND libs ${Nest_SOURCE_DIR}/tmx/deps/lib/libz.dll.a)

message(${libs})

FILE(GLOB CSources *.c)
set(SOURCE_FILES ${CSources} collisions.c collisions.h)
include_directories(. ${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR})
add_executable(Nest ${SOURCE_FILES})
target_link_libraries(Nest ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${libs})