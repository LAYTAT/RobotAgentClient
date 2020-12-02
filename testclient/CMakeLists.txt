cmake_minimum_required(VERSION 3.10)

project(ROBOTCLIENT)

set(CMAKE_CXX_STANDARD 11)
set(EXECUTABLE_OUTPUT_PATH ${ROBOTCLIENT_SOURCE_DIR}/../exe)

link_directories(${ROBOTCLIENT_SOURCE_DIR}/thirdparty/lib)
link_directories(${ROBOTCLIENT_SOURCE_DIR}/../behaviac3.6.39/lib)

FILE(GLOB_RECURSE SRC_CPP "src/*.cpp")
FILE(GLOB_RECURSE SRC_HEAD "include/*.h")
FILE(GLOB_RECURSE SRC_ROBOT "../behaviac3.6.39/exported/behaviac_generated/types/internal/*")

add_executable(robotclient "")

target_sources(robotclient PRIVATE 
	${SRC_CPP}
	${SRC_HEAD}
	${SRC_ROBOT}
)

target_include_directories(robotclient PRIVATE 
	${ROBOTCLIENT_SOURCE_DIR}/thirdparty/include
	${ROBOTCLIENT_SOURCE_DIR}/include
	${ROBOTCLIENT_SOURCE_DIR}/../behaviac3.6.39/exported/behaviac_generated/types
	${ROBOTCLIENT_SOURCE_DIR}/thirdparty/include/asio-1.10.8/include
)

target_compile_definitions(robotclient PRIVATE 
	-DDEBUG -D_DEBUG -DASIO_STANDALONE -DASIO_HAS_STD_CHRONO -DCURL_STATICLIB -DFOR_SERVER
)

target_compile_options(robotclient PRIVATE
)

target_link_libraries(robotclient PRIVATE
	"$<$<PLATFORM_ID:Linux>:behaviac_gcc_debug>"
	"$<$<PLATFORM_ID:Linux>:pthread>"
	"$<$<PLATFORM_ID:Windows>:libbehaviac_msvc_debug>"
)
