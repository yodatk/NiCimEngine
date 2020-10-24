/**
 * NNUE adapter: Functions wrappers to use the stockfish nnue libary
 */

#ifndef NICIMENGINE_NNUEEVAL_H
#define NICIMENGINE_NNUEEVAL_H

#include "./nnue/nnue.h"


/**
 * initial NNUE for evalutaion
 * @param fileName file of the NNUE
 */
void initNNUE(char* fileName);

/**
 * get NNUE evaluation for scoring directly
 */
int evaluateNNUE(int player,int* pieces, int* squares);
/**
 * get NNUE evaluation for scoring from given fen
 * @param fen - string represents the board to evaluate
 */
int evaluateFENNNUE(char* fen);



#endif //NICIMENGINE_NNUEEVAL_H
