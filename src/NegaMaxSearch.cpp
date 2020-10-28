#include "NegaMaxSearch.h"


/**
 * Search current board position according to time parameters and given depth, and print current best move and Principal variation nodes for each depth
 * @param depth depth of search
 */
void searchPosition(int depth) {


    // reset time, score , Principal variation variables, and move table
    long start = getTimeMs();
    int score = 0;
    nodes = 0;
    isStopped = 0;
    followPV = 0;
    scorePV = 0;

    // clear helper data structures for search
    memset(killerMoves, 0, sizeof(killerMoves));
    memset(historyMoves, 0, sizeof(historyMoves));
    memset(pvTable, 0, sizeof(pvTable));
    memset(pvLength, 0, sizeof(pvLength));

    // initial alpha beta bounds
    int alpha = -INFINITY;
    int beta = INFINITY;


    if (IS_USING_BOOK && IS_USING_BOOK_SUDO) {
        int bestMove = getBookMove();
        if (bestMove != 0) {
            pvTable[0][0] = bestMove;
        }
        else{
            IS_USING_BOOK = false;
        }
    }

    if (pvTable[0][0] == 0) {
        for (int currentDepth = 1; currentDepth <= depth; currentDepth++) {
            // iterative deepening -> search each depth to be ready for cancelation
            if (isStopped == 1) {
                // stop signal was sent -> stop searching and return best move so far
                break;
            }


            // enable follow PV flag
            followPV = 1;
            // calling search with current depth
            score = negamax(alpha, beta, currentDepth);


            if ((score <= alpha) || (score >= beta)) {
                // range was might have been too small, go to next level depth with initial range
                alpha = -INFINITY;
                beta = INFINITY;
                continue;
            }

            // set up the window for the next iteration
            alpha = score - 50;
            beta = score + 50;

            // if PV node is available
            if (pvLength[0]) {
                // Printing Principal variation , if there is mate -> print it too
                if (score > -MATE_VALUE && score < -MATE_SCORE) {
                    printf("info score mate %d depth %d nodes %lld time %ld pv ", -(score + MATE_VALUE) / 2 - 1,
                           currentDepth, nodes,
                           (getTimeMs() - start));
                } else if (score > MATE_SCORE && score < MATE_VALUE) {
                    printf("info score mate %d depth %d nodes %lld time %ld pv ", (MATE_VALUE - score) / 2 + 1,
                           currentDepth, nodes,
                           (getTimeMs() - start));
                } else {
                    printf("info score cp %d depth %d nodes %lld time %ld pv ", score, currentDepth, nodes,
                           (getTimeMs() - start));
                }


                for (int count = 0; count < pvLength[0]; count++) {
                    // print PV move
                    printMove(pvTable[0][count]);
                    printf(" ");
                }
                printf("\n");
            }
        }
    }



    // print best move
    printf("bestmove ");
    if (pvTable[0][0]) {
        printMove(pvTable[0][0]);
    } else {
        printf("(none)");
    }

    printf("\n");
}