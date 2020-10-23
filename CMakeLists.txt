cmake_minimum_required(VERSION 3.17)
project(NiCimEngine)

set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()


set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-Ofast")
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast")

add_executable(NiCimEngine src/main.cpp src/uci.cpp src/uci.h src/squares.h src/pieces.h src/pieces.cpp src/board.cpp src/board.h src/attacks.cpp src/attacks.h src/evaluation.cpp src/evaluation.h src/timeController.cpp src/timeController.h src/Randomnubers.cpp src/Randomnubers.h src/bitboard.cpp src/bitboard.h src/zorbiestKeys.cpp src/zorbiestKeys.h src/EnumsAndConstans.h src/squares.cpp src/magicNumbers.cpp src/magicNumbers.h src/perftTest.cpp src/perftTest.h src/Search.cpp src/Search.h src/TranspositionTable.cpp src/TranspositionTable.h src/NegaMaxSearch.cpp src/NegaMaxSearch.h ${SRCS} resources/resources.rc)