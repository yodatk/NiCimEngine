cmake_minimum_required(VERSION 3.17)
project(NiCimEngine)

set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()


set(CMAKE_CXX_FLAGS "-Wall -Wextra -static-libstdc++ -static-libgcc")
set(CMAKE_CXX_FLAGS_DEBUG "-Ofast")
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -fomit-frame-pointer -DUSE_SSE41 -msse4.1 -DUSE_SSSE3 -mssse3 -DUSE_SSE2 -msse2 -DUSE_SSE -msse")

add_executable(NiCimEngine src/main.cpp src/Uci.cpp src/Uci.h src/Squares.h src/Pieces.h src/Pieces.cpp src/Board.cpp src/Board.h src/Attacks.cpp src/Attacks.h src/Evaluation.cpp src/Evaluation.h src/TimeController.cpp src/TimeController.h src/RandomNumbers.cpp src/RandomNumbers.h src/Bitboard.cpp src/Bitboard.h src/ZorbiestKeys.cpp src/ZorbiestKeys.h src/EnumsAndConstants.h src/Squares.cpp src/MagicNumbers.cpp src/MagicNumbers.h src/PerftTest.cpp src/PerftTest.h src/Search.cpp src/Search.h src/TranspositionTable.cpp src/TranspositionTable.h src/NegaMaxSearch.cpp src/NegaMaxSearch.h src/nnue/misc.cpp src/nnue/misc.h src/nnue/nnue.cpp src/nnue/nnue.h src/NnueEval.cpp src/NnueEval.h src/PolyKeys.cpp src/PolyKeys.h src/PolyBook.cpp src/PolyBook.h ${SRCS} resources/resources.rc)