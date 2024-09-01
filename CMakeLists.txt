cmake_minimum_required(VERSION 3.10)
project(aptly-grpc-lib VERSION 1.0.0 LANGUAGES CXX)

add_custom_target(GenerateSources
        COMMAND /opt/bin/protoc -I /usr/share/aptly  --cpp_out=.  /usr/share/aptly/service.proto
        COMMENT "Generating grpc source"
)

add_library(${PROJECT_NAME} SHARED)
target_sources(${PROJECT_NAME} PRIVATE "src/aptly_grpc.cpp")

add_dependencies(${PROJECT_NAME} GenerateSources)

install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)
install(FILES ${CMAKE_BINARY_DIR}/service.pb.h DESTINATION /usr/include/aptly)
install(FILES ${CMAKE_SOURCE_DIR}/src/aptly_grpc.h DESTINATION /usr/include/aptly)