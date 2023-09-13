cmake_minimum_required(VERSION 3.10)
project(xsl LANGUAGES CXX)

add_executable(xsl main.cpp)
target_compile_features(xsl PRIVATE cxx_std_20)
target_include_directories(xsl PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)