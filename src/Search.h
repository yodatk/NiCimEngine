/**
 * In charge of Search Helpers functions, and helping variables for search
 */

#ifndef NISSIMENGINECPP_SEARCH_H
#define NISSIMENGINECPP_SEARCH_H


/**
 * Max ply to reach in search
 */
#define MAX_PLY 64

/**
 * Value to define Mate scores
 */
#define INFINITY 50000
#define MATE_VALUE 49000
#define MATE_SCORE 48000

#include "PerftTest.h"
#include "Evaluation.h"

/**
 * MOST-VALUABLE-VICTIM LEAST-VALUABLE-ATTACKER Table
 */
static int MVV_LVA[12][12] = {
        {105, 205, 305, 405, 505, 605, 105, 205, 305, 405, 505, 605},
        {104, 204, 304, 404, 504, 604, 104, 204, 304, 404, 504, 604},
        {103, 203, 303, 403, 503, 603, 103, 203, 303, 403, 503, 603},
        {102, 202, 302, 402, 502, 602, 102, 202, 302, 402, 502, 602},
        {101, 201, 301, 401, 501, 601, 101, 201, 301, 401, 501, 601},
        {100, 200, 300, 400, 500, 600, 100, 200, 300, 400, 500, 600},

        {105, 205, 305, 405, 505, 605, 105, 205, 305, 405, 505, 605},
        {104, 204, 304, 404, 504, 604, 104, 204, 304, 404, 504, 604},
        {103, 203, 303, 403, 503, 603, 103, 203, 303, 403, 503, 603},
        {102, 202, 302, 402, 502, 602, 102, 202, 302, 402, 502, 602},
        {101, 201, 301, 401, 501, 601, 101, 201, 301, 401, 501, 601},
        {100, 200, 300, 400, 500, 600, 100, 200, 300, 400, 500, 600}
};

/**
 * Principal Variation Seaech Length array according to ply
 */
extern int pvLength[MAX_PLY];

/**
 * Principal Variation Search moveTable according to given ply
 */
extern int pvTable[MAX_PLY][MAX_PLY];

/**
 * followPV ->  flag to determine if to go according Principle variation ScorePV -> flag to determine if to score moves according to principale variation
 */
extern int followPV, scorePV;


/**
 * Killer moves table for scoring moves and sorting them better
 */
extern int killerMoves[2][MAX_PLY];

/**
 * History moves table for scoring moves and sorting them better
 */
extern int historyMoves[12][64];

/**
 * Integer constant for LMR search to mesure searched moves
 */
extern const int fullDepthMoves;

/**
 * Integer constant to check for the LMR search to check if the current search is deep enough
 */
extern const int reductionLimit;


/**
 * scoring current move according to PV search, Killer Moves, and History moves
 * @param move Integer represents current move to check
 * @return score for the given move accorfing to board stats, Principal variation, killer moves, MVV-LVA , and history moves
 */
static inline int scoreMove(int move) {
    if (scorePV) {
        // make sure we are dealing with PV move
        if (pvTable[0][ply] == move) {
            // found PV move, disable flag and return score to be searched first
            scorePV = 0;
            return 20000;
        }
    }


    if (getMoveCapture(move)) {
        // if current move is a capture move, find the attacker and victim and score accordingly
        int target_piece = P;

        int start_piece, end_piece;

        if (side == WHITE) {
            start_piece = p;
            end_piece = k;
        } else {
            start_piece = P;
            end_piece = K;
        }

        for (int bb_piece = start_piece; bb_piece <= end_piece; bb_piece++) {
            if (getBit(bitboards[bb_piece], getMoveTarget(move))) {
                target_piece = bb_piece;
                break;
            }
        }

        return MVV_LVA[getMovePiece(move)][target_piece] + 10000;
    } else {
        // if it's not a capture move, check if it's a killer or history move
        if (killerMoves[0][ply] == move) {
            return 9000;
        } else if (killerMoves[1][ply] == move) {
            return 8000;
        } else {
            return historyMoves[getMovePiece(move)][getMoveTarget(move)];
        }

    }

}

/**
 * Sort Moves according to their posibillty to be Principal variation Node. (PV first, Then capture move, than killerm moves, then history moves) using bubble sort
 * @param moveList list of all the possible moves from the current situation
 * @return flag '1' represent the sort is finished
 */
static inline int sortMoves(moves *moveList,int bestMove) {
    int moveScores[moveList->count];
    for (int i = 0; i < moveList->count; i++) {
        // if best move -> give A LOT of score
        if(bestMove == moveList->moves[i]){
            moveScores[i] = 30000;
        }
        else{
            // for each move, calculate score
            moveScores[i] = scoreMove(moveList->moves[i]);
        }
    }


    // bubble sort according to score. bigger score first, smaller score last
    for (int current_move = 0; current_move < moveList->count; current_move++) {
        for (int next_move = current_move + 1; next_move < moveList->count; next_move++) {

            if (moveScores[current_move] < moveScores[next_move]) {
                // swap scores
                int tempScore = moveScores[current_move];
                moveScores[current_move] = moveScores[next_move];
                moveScores[next_move] = tempScore;

                // swap moves
                int tempMove = moveList->moves[current_move];
                moveList->moves[current_move] = moveList->moves[next_move];
                moveList->moves[next_move] = tempMove;
            }
        }
    }
    return 1;
}

/**
 * QuietSearch to solve the HORIZON PROBLEM. keep searching best moves until there are no reasonable captures possible
 * @param alpha -  score gathered for fail low node
 * @param beta - score gathered for fail high node
 * @return score of the move that was found to be best from the current situation
 */
static inline int quietSearch(int alpha, int beta) {

    if ((nodes & 2047) == 0) {
        // checks if the user / GUI signal to stop the search
        communicate();
    }

    nodes++;

    if (ply > MAX_PLY - 1) {
        // searched to deep -> evaluate position
        return evaluate();
    }

    int evaluation = evaluate();


    if (evaluation >= beta) {
        // current position evaluation fails high.
        return beta;
    }

    if (evaluation > alpha) {
        // found a better move, change alpha value
        alpha = evaluation;
    }

    // generate and sort next move possible
    moves moveList[1];
    generateMoves(moveList);
    sortMoves(moveList,0);


    for (int count = 0; count < moveList->count; count++) {
        // saving board state
        copyBoard();

        ply++;

        // adding move to repetition table to avoid 3-fold repetition
        repetitionIndex++;
        repetitionTable[repetitionIndex] = hashKey;

        if (makeMove(moveList->moves[count], CAPTURES_ONLY) == 0) {
            // if the current chosen move is not a capture move -> skip it
            ply--;
            repetitionIndex--;
            continue;
        }

        // score current capture move and restoring depth and repetition table
        int score = -quietSearch(-beta, -alpha);
        ply--;

        repetitionIndex--;

        // restoring board
        restoreBoard();

        // check if search was stop
        if (isStopped == 1) {
            return 0;
        }


        if (score > alpha) {
            // current move is better
            alpha = score;


            if (score >= beta) {
                // current move failed high
                return beta;
            }
        }
    }

    // return current score of fail low score ( or pv if edited)
    return alpha;
}


/**
 * Enabling principal variation search and scoring if PV move is possible. otherwise, turning it off
 * @param moveList list of integers represents current possible moves
 */
static inline void enablePvScoring(moves *moveList) {

    followPV = 0;
    for (int count = 0; count < moveList->count; count++) {
        if (pvTable[0][ply] == moveList->moves[count]) {
            scorePV = 1;
            followPV = 1;
        }
    }
}


/**
 * Print all possible moves in current board state
 * @param moveList list of integers represents encoded moves
 */
void printMoveScores(moves *moveList);


#endif //NISSIMENGINECPP_SEARCH_H
