cmake_minimum_required(VERSION 3.25)
project(algoritmo_dyv)

set(CMAKE_CXX_STANDARD 17)

add_executable(dyv dyv.cpp)
add_executable(especifico especifico.cpp)
add_executable(dyv_sin_umbral dyv_sin_umbral.cpp)
add_executable(dyv_con_umbral dyv_con_umbral.cpp)
add_executable(generador generador.cpp)
