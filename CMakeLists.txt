cmake_minimum_required(VERSION 3.10)
project(aptly-grpc-lib VERSION 1.0.0 LANGUAGES CXX)

include(common.cmake)

# Proto file
get_filename_component(proto "/usr/share/aptly/service.proto" ABSOLUTE)
get_filename_component(proto_path "${proto}" PATH)

# Generated sources
set(proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/service.pb.cc")
set(proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/service.pb.h")
set(grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/service.grpc.pb.cc")
set(grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/service.grpc.pb.h")
add_custom_command(
        OUTPUT "${proto_srcs}" "${proto_hdrs}" "${grpc_srcs}" "${grpc_hdrs}"
        COMMAND ${_PROTOBUF_PROTOC}
        ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${proto}"
        DEPENDS "${proto}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

set(HEADERS
        "src/repositoryClient.h"
        "src/filesClient.h"
        "src/aptlyClient.h"
        ${grpc_hdrs}
        ${proto_hdrs}
)

set(SOURCES
        "src/repositoryClient.cpp"
        "src/filesClient.cpp"
        "src/aptlyClient.cpp"
        ${grpc_srcs}
        ${proto_srcs}
)

add_library(${PROJECT_NAME} SHARED ${HEADERS} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
        ${_REFLECTION}
        ${_GRPC_GRPCPP}
        ${_PROTOBUF_LIBPROTOBUF})

install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)
install(FILES ${CMAKE_BINARY_DIR}/service.pb.h DESTINATION /usr/include/aptly)
install(FILES
        ${CMAKE_SOURCE_DIR}/src/aptlyClient.h
        ${CMAKE_SOURCE_DIR}/src/filesClient.h
        ${CMAKE_SOURCE_DIR}/src/repositoryClient.h
        DESTINATION
        /usr/include/aptly)
install(FILES ${grpc_srcs} DESTINATION /usr/include/aptly)
install(FILES ${grpc_hdrs} DESTINATION /usr/include/aptly)
install(FILES ${proto_srcs} DESTINATION /usr/include/aptly)
install(FILES ${proto_hdrs} DESTINATION /usr/include/aptly)
