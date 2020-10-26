/**
 * in charge of using nnue evaluation by https://github.com/dshawul/nnue-probe in NiCim Engine
 */
#include "NnueEval.h"




/**
 * initial NNUE for evalutaion
 * @param fileName file of the NNUE
 */
void initNNUE(char* fileName){
    // call prob lib function
    nnue_init(fileName);
}

/**
 * get NNUE evaluation for scoring directly
 */
int evaluateNNUE(int player,int* pieces, int* squares){
    // call prob lib function
    return nnue_evaluate(player,pieces,squares);

}
/**
 * get NNUE evaluation for scoring from given fen
 * @param fen - string represents the board to evaluate
 */
int evaluateFENNNUE(char* fen){
    // call prob lib function
    return nnue_evaluate_fen(fen);
}