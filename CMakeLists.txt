cmake_minimum_required(VERSION 3.17)
project(NiCimEngine)

set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-Ofast")
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast")

add_executable(NiCimEngine main.cpp uci.cpp uci.h squares.h pieces.h pieces.cpp board.cpp board.h attacks.cpp attacks.h evaluation.cpp evaluation.h timeController.cpp timeController.h Randomnubers.cpp Randomnubers.h bitboard.cpp bitboard.h zorbiestKeys.cpp zorbiestKeys.h EnumsAndConstans.h squares.cpp magicNumbers.cpp magicNumbers.h perftTest.cpp perftTest.h Search.cpp Search.h TranspositionTable.cpp TranspositionTable.h NegaMaxSearch.cpp NegaMaxSearch.h)