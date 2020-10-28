/**
 * in charge of using nnue evaluation by https://github.com/dshawul/nnue-probe in NiCim Engine
 */
#include <cstdio>
#include "NnueEval.h"




/**
 * Flag To Determine if NNUE is Present or not
 */
int IS_NNUE = true;
/**
 * initial NNUE for evalutaion
 * @param fileName file of the NNUE
 */
void initNNUE(char* fileName){
    FILE *file = fopen(NNUE_FILE,"r");
    if(file){
        fclose(file);
        // file exists call prob lib function
        nnue_init(fileName);
        printf("IS_NNUE=true\n");
        IS_NNUE = true;
    } else {
        // file doesn't exist
        fclose(file);
        printf("IS_NNUE=false\n");
        IS_NNUE = false;
    }

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