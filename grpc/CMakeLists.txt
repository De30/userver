cmake_minimum_required(VERSION 3.8)

project(yandex-taxi-userver-grpc)

find_package(Grpc REQUIRED)
include(GrpcTargets)

file(GLOB_RECURSE PROTOS ${CMAKE_CURRENT_SOURCE_DIR}/*.proto)

file(GLOB_RECURSE SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
  

add_grpc_library(${PROJECT_NAME}_unittest_proto PROTOS ${PROTOS})

file(GLOB_RECURSE UNIT_TEST_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/*_test.cpp
)
list(REMOVE_ITEM SOURCES ${UNIT_TEST_SOURCES})

#file(GLOB_RECURSE BENCH_SOURCES
#  ${CMAKE_CURRENT_SOURCE_DIR}/*_benchmark.cpp
#)
#list (REMOVE_ITEM SOURCES ${BENCH_SOURCES})

add_library(${PROJECT_NAME} STATIC ${SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_include_directories (${PROJECT_NAME} SYSTEM
  PRIVATE
    $<TARGET_PROPERTY:yandex-taxi-userver-core,INCLUDE_DIRECTORIES>
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    yandex-taxi-userver-core
    Grpc
  PRIVATE
    yandex-taxi-userver-uboost-coro # uses Future
)

add_executable(${PROJECT_NAME}_unittest ${UNIT_TEST_SOURCES})
target_include_directories (${PROJECT_NAME}_unittest SYSTEM PRIVATE
    $<TARGET_PROPERTY:${PROJECT_NAME},INCLUDE_DIRECTORIES>
    ${CMAKE_CURRENT_BINARY_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/src
)
target_link_libraries(${PROJECT_NAME}_unittest PUBLIC
  utest
  ${PROJECT_NAME}
  $<TARGET_PROPERTY:Grpc,INTERFACE_LINK_LIBRARIES>
  PRIVATE ${PROJECT_NAME}_unittest_proto
)
add_google_tests(${PROJECT_NAME}_unittest)

#add_executable(${PROJECT_NAME}_benchmark ${BENCH_SOURCES})
#set_target_properties(${PROJECT_NAME}_benchmark PROPERTIES LINKER_LANGUAGE CXX)
#target_link_libraries(${PROJECT_NAME}_benchmark PRIVATE
#    ubench
#    ${PROJECT_NAME}
#)
#target_include_directories(${PROJECT_NAME}_benchmark
#    SYSTEM PRIVATE
#    ${CMAKE_CURRENT_SOURCE_DIR}/src
#)