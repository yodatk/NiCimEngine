
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
}

/**
 * debug purpuses main
 */
 int mainDebug(){
     initAll();
     parseFen(start_position);
     printBoard();
     // loaded file
     initNNUE("nn-04cf2b4ed1da.nnue");
     printf("nnue eval: %d ",evaluateFENNNUE(start_position));
     return 0;
 }

 int mainRelease(){

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

     return mainDebug();

    //return mainRelease();
}
