cmake_minimum_required(VERSION 3.17)
project(NiCimEngine)

set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_COMPILER "C:\\Program Files\\mingw-w64\\x86_64-8.1.0-posix-seh-rt_v6-rev0\\mingw64\\bin\\g++")


set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-Ofast")
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast")

add_executable(NiCimEngine src/main.cpp src/Uci.cpp src/Uci.h src/Squares.h src/Pieces.h src/Pieces.cpp src/Board.cpp src/Board.h src/Attacks.cpp src/Attacks.h src/Evaluation.cpp src/Evaluation.h src/TimeController.cpp src/TimeController.h src/RandomNumbers.cpp src/RandomNumbers.h src/Bitboard.cpp src/Bitboard.h src/ZorbiestKeys.cpp src/ZorbiestKeys.h src/EnumsAndConstants.h src/Squares.cpp src/MagicNumbers.cpp src/MagicNumbers.h src/PerftTest.cpp src/PerftTest.h src/Search.cpp src/Search.h src/TranspositionTable.cpp src/TranspositionTable.h src/NegaMaxSearch.cpp src/NegaMaxSearch.h ${SRCS} resources/resources.rc)