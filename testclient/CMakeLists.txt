cmake_minimum_required(VERSION 3.10)

project(ROBOTCLIENT)

set(CMAKE_CXX_STANDARD 11)

set(EXECUTABLE_OUTPUT_PATH ./build)



FILE(GLOB_RECURSE SRC_CPP "src/*.cpp")
FILE(GLOB_RECURSE SRC_NETWORK "src/SocketNetwork/*.cpp")
FILE(GLOB_RECURSE SRC_HEAD "include/*.h")
FILE(GLOB_RECURSE SRC_PROTO "include/proto/*.cc")
FILE(GLOB_RECURSE SRC_ROBOT "../behaviac3.6.39/exported/behaviac_generated/types/internal/*")

add_executable(robotclient "")

target_sources(robotclient PRIVATE 
	${SRC_CPP}
	${SRC_HEAD}
	${SRC_ROBOT}
	${SRC_NETWORK}
	${SRC_PROTO}
)

include_directories(./include/proto)

target_include_directories(robotclient PRIVATE
	${ROBOTCLIENT_SOURCE_DIR}/include
)

target_compile_definitions(robotclient PRIVATE 
	-DDEBUG -D_DEBUG -DASIO_STANDALONE -DASIO_HAS_STD_CHRONO -DCURL_STATICLIB -DFOR_SERVER
)

target_compile_options(robotclient PRIVATE
)

target_link_libraries(robotclient
		protobuf
		pthread
		)