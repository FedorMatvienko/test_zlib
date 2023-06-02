# Определение библиотек и опций компиляции
cmake_minimum_required( VERSION 3.8.0 )

# Установка стандарта для компилятора, чтобы программа видела стандарт +17
set( CMAKE_CXX_STANDARD 20 )
# Установка названия проекта
set ( project test_zlib )
project ( ${project} )

add_executable( ${project} main.cpp)
target_link_libraries(${project} -lz )