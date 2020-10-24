
#ifndef NISSIMENGINECPP_NEGAMAXSEARCH_H
#define NISSIMENGINECPP_NEGAMAXSEARCH_H

#include "Search.h"
#include "TranspositionTable.h"


/**
 * Main Search Function with:
 *  # Alpha - Beta pruning
 *  # Null pruning
 *  # Late Move Reduction
 *  # Principal Variation Search
 * @param alpha Integer represents the current alpha score for low score pruning
 * @param beta  Integer represents the current beta score for high score pruning
 * @param depth
 * @return Score of the best move from current situation on the board
 */
static inline int negamax(int alpha, int beta, int depth) {
    // initializing length of current variation
    pvLength[ply] = ply;


    int score;

    int bestMove = 0;

    int hashFlag = ALPHA;

    if ((ply && isRepetition()) || fiftyRuleCounter >= 100) {
        // if repetition is found \ 50 move rule found -> return draw value
        return 0;
    }


    // checking if current node is PV or not
    int pvNode = beta - alpha > 1;


    if (ply && (score = readHashEntry(alpha, beta, &bestMove, depth)) != NO_HASH_FOUND && pvNode == 0) {
        // if search deep is bigger than 0 AND hash entry was found, AND current node is not a PV node -> return score found in entry
        return score;
    }


    if ((nodes & 2047) == 0) {
        // check for GUI/user input
        communicate();
    }


    if (depth == 0) {

        // no more depth to search in negamax, go to quiet search(escape condition)
        return quietSearch(alpha, beta);
    }


    if (ply > MAX_PLY - 1) {
        // in case we are to deep in the search -> return evaluation of the board
        return evaluate();
    }

    nodes++;

    // checking for checks
    int isInCheck = isSquareAttacked((side == WHITE) ? getLSBIndex(bitboards[K]) : getLSBIndex(bitboards[k]),
                                     side ^ 1);


    if (isInCheck) {
        // if in check -> scan deeper
        depth++;
    }


    int legalMoves = 0;

    // null move pruning -> making another move from oppononet before continue evaluating to prune more moves
    if (depth >= 3 && isInCheck == 0 && ply) {

        copyBoard();
        ply++;

        // updating repetition table
        repetitionIndex++;
        repetitionTable[repetitionIndex] = hashKey;

        // switch sides and fix hashing for switching sides
        if (enpassant != NO_SQUARE)
            hashKey ^= enpassantKeys[enpassant];
        enpassant = NO_SQUARE;

        side ^= 1;
        hashKey ^= sideKey;

        // searching with smaller depth, with considiration on limit reduction
        score = -negamax(-beta, -beta + 1, depth - 1 - 2);


        ply--;
        repetitionIndex--;
        restoreBoard();


        if (isStopped == 1) {
            // flag to stop, quit searching
            return 0;
        }

        if (score >= beta) {
            // node (position) fails high
            return beta;
        }

    }

    // razoring
    if (!pvNode && !isInCheck && depth <= 3) {
        // get static eval and add first bonus
        score = evaluate() + 125;

        // define new score
        int newScore;

        // static evaluation indicates a fail-low node
        if (score < beta) {
            // on depth 1
            if (depth == 1) {
                // get quiscence score
                newScore = quietSearch(alpha, beta);

                // return quiescence score if it's greater then static evaluation score
                return (newScore > score) ? newScore : score;
            }

            // add second bonus to static evaluation
            score += 175;

            // static evaluation indicates a fail-low node
            if (score < beta && depth <= 2) {
                // get quiscence score
                newScore = quietSearch(alpha, beta);

                // quiescence score indicates fail-low node
                if (newScore < beta)
                    // return quiescence score if it's greater then static evaluation score
                    return (newScore > score) ? newScore : score;
            }
        }
    }


    moves moveList[1];
    generateMoves(moveList);


    if (followPV) {
        //following Principal variation Line(PV) ->  enable PV move scoring
        enablePvScoring(moveList);
    }

    sortMoves(moveList, bestMove);

    // number of moves searched in a move list
    int movesSearched = 0;


    for (int count = 0; count < moveList->count; count++) {

        copyBoard();
        ply++;

        // updating repetition table
        repetitionIndex++;
        repetitionTable[repetitionIndex] = hashKey;


        if (makeMove(moveList->moves[count], ALL_MOVES) == 0) {
            // if invalid move -> go back to another move
            ply--;
            repetitionIndex--;
            continue;
        }

        legalMoves++;


        if (movesSearched == 0) {
            // no search occured yet -> search normally
            score = -negamax(-beta, -alpha, depth - 1);
        }
            // Late Move Reduction
        else {
            // condition to consider LMR
            if (
                    movesSearched >= fullDepthMoves &&
                    depth >= reductionLimit &&
                    isInCheck == 0 &&
                    getMoveCapture(moveList->moves[count]) == 0 &&
                    getMovePromoted(moveList->moves[count]) == 0)
                // search current move with reduced depth:
                score = -negamax(-alpha - 1, -alpha, depth - 2);

                // ensure full depth is going to be done
            else
                score = alpha + 1;

            // principle variation search PVS
            if (score > alpha) {
                //Focusing on Principal Variation
                score = -negamax(-alpha - 1, -alpha, depth - 1);

                // If Reduction was wrong -> try and fixed it with search with more depth and normal alpha beta score bounds
                if ((score > alpha) && (score < beta)) {
                    score = -negamax(-beta, -alpha, depth - 1);
                }

            }
        }

        ply--;
        repetitionIndex--;
        restoreBoard();


        if (isStopped == 1) {
            // Stop search due to signal by user \ GUI
            return 0;
        }


        movesSearched++;


        if (score > alpha) {
            // found a better move -> mark this move as Principal variation Node
            hashFlag = PV;

            bestMove = moveList->moves[count];

            if (getMoveCapture(moveList->moves[count]) == 0) {
                // if not a capture -> store as history move
                historyMoves[getMovePiece(moveList->moves[count])][getMoveTarget(
                        moveList->moves[count])] += depth;

            }

            // PV node (position), writing it to PV table
            alpha = score;
            pvTable[ply][ply] = moveList->moves[count];
            for (int nextPly = ply + 1; nextPly < pvLength[ply + 1]; nextPly++) {
                // copy move from deeper ply into a current ply's line
                pvTable[ply][nextPly] = pvTable[ply + 1][nextPly];
            }
            pvLength[ply] = pvLength[ply + 1];


            if (score >= beta) {
                // storing move as beta
                writeHashEntry(beta, depth, bestMove, BETA);


                if (getMoveCapture(moveList->moves[count]) == 0) {
                    // on quiet moves -> store killer moves
                    killerMoves[1][ply] = killerMoves[0][ply];
                    killerMoves[0][ply] = moveList->moves[count];
                }

                // node (position) fails high
                return beta;
            }
        }
    }


    if (legalMoves == 0) {
        // we don't have any legal moves to make in the current postion -> check for mate or stalmate
        if (isInCheck) {
            // Mate: returning MATE_VALUE + ply to consider fast mate first
            return -MATE_VALUE + ply;
        } else {
            // StalMate ->  return draw score
            return 0;
        }

    }

    // store hash entry with the score equal to alpha
    writeHashEntry(alpha, depth, bestMove, hashFlag);

    // return fail low score
    return alpha;
}


/**
 * Search current board position according to time parameters and given depth, and print current best move and Principal variation nodes for each depth
 * @param depth depth of search
 */
void searchPosition(int depth);

#endif //NISSIMENGINECPP_NEGAMAXSEARCH_H
