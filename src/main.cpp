
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

    // init delay time - for online matches or local matches
    initDelayTime();
    printf("DELAY_TIME: %d\n",DELAY_TIME);

    // initilizing NNUE file
    initNNUE(NNUE_FILE);

    // initializing book file
    IS_USING_BOOK_SUDO = initPolyBook();
    IS_USING_BOOK = IS_USING_BOOK_SUDO;

}

/**
 * debug purpuses main
 */
int mainDebug() {
    initAll();
    return 0;
}

int mainRelease() {

    initAll();


    UCILoop();

    // free allocated hash
    free(hashTable);
    cleanPolyBook();

    return 0;
}

/**
 * MAIN
 */
int main() {

    // return mainDebug();

    return mainRelease();
}
