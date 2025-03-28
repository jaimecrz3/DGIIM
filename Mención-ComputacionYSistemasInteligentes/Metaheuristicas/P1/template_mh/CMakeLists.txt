cmake_minimum_required(VERSION 3.28.3)

PROJECT(mh_p1 CXX)

file(GLOB C_SOURCES
  "src/*.cpp"
)
INCLUDE_DIRECTORIES("common" "inc")

ADD_EXECUTABLE(main "main.cpp" ${C_SOURCES})
