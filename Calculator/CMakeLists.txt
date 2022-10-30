cmake_minimum_required(VERSION 3.8)

project(Calcualtor)

set(SOURCES
        main.cpp
        Calculator.cpp
)

set(HEADERS
      Calculator.h
	Operation.h
)

add_executable(${SOURCES} ${HEADERS})

set_property(TARGET Calc_ver2 PROPERTY CXX_STANDARD 20)
