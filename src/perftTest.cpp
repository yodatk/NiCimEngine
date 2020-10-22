
#include "perftTest.h"

/**
 * counter of all the "leaf" positions in a search
 */
U64 nodes = 0;


/**
 * performing perth test with time measuring and listing all initial moves with their connected leaf nodes
 * @param depth depth to search node in
 */
void perftTest(int depth) {
    printf("\n     Performance test\n\n");


    moves moveList[1];


    generateMoves(moveList);

    // init start time for measuring
    long start = getTimeMs();


    for (int moveCount = 0; moveCount < moveList->count; moveCount++) {

        copyBoard();

        // make move
        if (!makeMove(moveList->moves[moveCount], ALL_MOVES)) {
            // invalid move -> skip to the next move
            continue;
        }

        // to calculate all the nodes from current move
        U64 cummulative_nodes = nodes;

        // search leaves nodes from current move
        perftDriver(depth - 1);

        // calculate nodes to display for move
        long old_nodes = nodes - cummulative_nodes;


        restoreBoard();


        printf("     move: %s%s%c  nodes: %ld\n", squareToCoordinates[getMoveSource(moveList->moves[moveCount])],
               squareToCoordinates[getMoveTarget(moveList->moves[moveCount])],
               getMovePromoted(moveList->moves[moveCount]) ? getPromotedPiece(
                       getMovePromoted(moveList->moves[moveCount])) : ' ',
               old_nodes);
    }

    printf("\n    Depth: %d\n", depth);
    printf("    Nodes: %lld\n", nodes);
    printf("     Time: %ld\n\n", getTimeMs() - start);
}