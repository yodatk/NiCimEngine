#include "Search.h"


/**
 * Principal Variation Seaech Length array according to ply
 */
int pvLength[MAX_PLY];

/**
 * Principal Variation Search moveTable according to given ply
 */
int pvTable[MAX_PLY][MAX_PLY];

/**
 * followPV ->  flag to determine if to go according Principle variation ScorePV -> flag to determine if to score moves according to principale variation
 */
int followPV, scorePV;


/**
 * Killer moves table for scoring moves and sorting them better
 */
int killerMoves[2][MAX_PLY];

/**
 * History moves table for scoring moves and sorting them better
 */
int historyMoves[12][64];

/**
 * Integer constant for LMR search to mesure searched moves
 */
const int fullDepthMoves = 4;

/**
 * Integer constant to check for the LMR search to check if the current search is deep enough
 */
const int reductionLimit = 3;

/**
 * Print all possible moves in current board state
 * @param moveList
 */
void printMoveScores(moves *moveList) {
    printf("     Move scores:\n\n");

    // loop over moves within a move list
    for (int count = 0; count < moveList->count; count++) {
        printf("     move: ");
        printMove(moveList->moves[count]);
        printf(" score: %d\n", scoreMove(moveList->moves[count]));
    }
}


