
#include <cstdlib>

#ifdef WIN64

#include <windows.h>

#else
#include <sys/time.h>
#endif

#include "Uci.h"


/**
 * Init all the data that necessary to start the engine loop
 */
void initAll() {
    // init all pieces attacks
    initLeapersAttacks();

    initSlidersAttacks(bishop);
    initSlidersAttacks(rook);

    // init Hashing Keys
    initHashKeys();

    // init all Evaluation data
    initEvaluationMasks();

    // init Hash Table with default size value
    initHashTable(DEFAULT_HASH_SIZE);
    initNNUE(NNUE_FILE);
}

/**
 * debug purpuses main
 */
int mainDebug() {
    initAll();
    parseFen(start_position);
    printBoard();
    // loaded file
    initNNUE("nn-04cf2b4ed1da.nnue");


    int pieces[33];
    int squares[33];
    nnue_input(pieces, squares);
    int score = evaluateNNUE(side, pieces, squares);
    printf("nnue eval: %d\n", evaluateFENNNUE(start_position));
    printf("score: %d\n", score);

    int eval_score = evaluate();
    printf("eval_score::: %d\n", eval_score);

    return 0;
}

int mainRelease() {

    initAll();


    UCILoop();

    // free allocated hash
    free(hashTable);

    return 0;
}

/**
 * MAIN
 */
int main() {

    // return mainDebug();

    return mainRelease();
}
