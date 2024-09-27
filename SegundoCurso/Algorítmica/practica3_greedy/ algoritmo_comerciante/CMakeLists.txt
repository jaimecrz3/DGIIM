cmake_minimum_required(VERSION 3.25)
project(_algoritmo_comerciante)

set(CMAKE_CXX_STANDARD 17)


add_executable(pvc1 pvc1.cpp)
add_executable(generador-pvc generador-pvc.cpp)
add_executable(pvc2 pvc2.cpp)
add_executable(pvc3 pvc3.cpp)
add_executable(algoritmo_catering ../algoritmo_catering/algoritmo_catering.cpp)
add_executable(generador ../algoritmo_catering/generador.cpp)

