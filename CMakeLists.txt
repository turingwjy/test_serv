#set(CMAKE_CXX_FLAGS "-fpermissive -p -g -O2 -Wl,-rpath=../../libc/ -D__STDC_LIMIT_MACROS")
set(CMAKE_CXX_FLAGS "-fpermissive -g -Wl,-rpath=../../libc/ -D__STDC_LIMIT_MACROS -Wformat=0")
#SET(CMAKE_BUILD_TYPE Release)
SET(CMAKE_BUILD_TYPE Debug)

file(GLOB sources_TEST *.cpp) 

set(Gameserver_STAT_SRCS
  ${sources_TEST}
 # ${sources_SHARE_DATA}
 # ${sources_HBASE}
)

message(status " This is the server_share srcs:" ${sources_SERVER_SHARE})
message(status " This is the share srcs:" ${sources_SHARE})
#message(status " This is the hbase srcs:" ${sources_HBASE})
message(status " This is the srcs:" ${sources})

#include_directories(
#  ${CMAKE_CURRENT_SOURCE_DIR}/../server_share
#  ${CMAKE_CURRENT_SOURCE_DIR}/../server_share/protocol
#  ${CMAKE_CURRENT_SOURCE_DIR}/../server_share/database
#  ${CMAKE_CURRENT_SOURCE_DIR}/../server_share/util
#  ${CMAKE_CURRENT_SOURCE_DIR}/../server_share/network
#  ${CMAKE_CURRENT_SOURCE_DIR}/../share
#  ${CMAKE_CURRENT_SOURCE_DIR}/system
#  ${CMAKE_CURRENT_SOURCE_DIR}
#  /usr/local/include/mozjs-24
#  /usr/local/include/libbson-1.0
#  /usr/local/include/libmongoc-1.0
#  /usr/include/mono-2.0
#)
LINK_DIRECTORIES(
  /usr/local/lib
  /usr/lib
)
add_executable(Test ${Gameserver_STAT_SRCS})
target_link_libraries(Test ACE)
#target_link_libraries(Gameserver mysqlclient_r)
#target_link_libraries(Gameserver json_linux)
#target_link_libraries(Gameserver tcmalloc)
#target_link_libraries(Gameserver mozjs-24)
#target_link_libraries(Gameserver curl)
#target_link_libraries(Gameserver readline)
#target_link_libraries(Gameserver boost_system)
#target_link_libraries(Gameserver boost_filesystem)
#target_link_libraries(Gameserver boost_regex)
#target_link_libraries(Gameserver pthread) # require for g++ 4.8.2
#target_link_libraries(Gameserver z) # require for g++ 4.8.2
#Wtarget_link_libraries(Gameserver mongoc-1.0)
#target_link_libraries(Gameserver bson-1.0)
#target_link_libraries(Gameserver mono-2.0)
#target_link_libraries(Gameserver protobuf)
