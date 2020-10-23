/**
 * Performance test to check the engine does calculate all possible moves
 */

#ifndef NISSIMENGINECPP_PERFTTEST_H
#define NISSIMENGINECPP_PERFTTEST_H


#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "Board.h"
#include "TimeController.h"

/**
 * counter of all the "leaf" positions in a search
 */
extern U64 nodes;

/**
 * Counting all possible moves in a current situation, recursively tu count all possible nodes
 * @param depth depth to search leaf nodes from
 */
static inline void perftDriver(int depth) {
    if (depth == 0) {
        //got to a leaf node -> count it and exit
        nodes++;
        return;
    }

    // generate all possible moves and for each one search for leaf nodes
    moves moveList[1];
    generateMoves(moveList);

    for (int move_count = 0; move_count < moveList->count; move_count++) {

        copyBoard();

        if (!makeMove(moveList->moves[move_count], ALL_MOVES)) {
            // if move is not possible -> continue to next one
            continue;
        }

        // searching leaves nodes deeper
        perftDriver(depth - 1);


        restoreBoard();
    }
}

/**
 * performing perth test with time measuring and listing all initial moves with their connected leaf nodes
 * @param depth depth to search node in
 */
void perftTest(int depth);


#endif //NISSIMENGINECPP_PERFTTEST_H
