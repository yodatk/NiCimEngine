/**
 * In charge on writing, reading and initializing Transposition Table
 */
#ifndef NISSIMENGINECPP_TRANSPOSITIONTABLE_H
#define NISSIMENGINECPP_TRANSPOSITIONTABLE_H


#define NO_HASH_FOUND 100000

/**
 * Transposition Hash Flags to tag various entries
 */
#define PV 0
#define ALPHA 1
#define BETA 2


#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "Search.h"
#include "Evaluation.h"


/**
 * Total number of entries in Hash Table
 */
extern int hashEntries;

/**
 * Struct to define an entry in the Transposition Table
 */
typedef struct {
    /**
     * Board hash Id
     */
    U64 hashKey;
    /**
     * Entry search depth
     */
    int depth;
    /**
     * Flag the type of node (fail-low/fail-high/PV)
     */
    int flag;
    /**
     * Score of current board situation
     */
    int score;
    /**
     * storing best move if necessary
     */
    int bestMove;
} TTEntry;

/**
 * HashTable / Transposition Table
 */
extern TTEntry *hashTable;


/**
 * Clearing Hash Table from all data, and free all related allocated data
 */
void clearHashTable();

/**
 * Initializing hash table
 * @param mb max size of allocated data for the table in MB
 */
void initHashTable(int mb);

/**
 * Read Hash Entry with given parameters if possible
 * @param alpha Integer represent the alpha score of current situation
 * @param beta  Integer represent the beta score of the current situation
 * @param bestMove address of the integer to put the value of the entry best move into
 * @param depth depth of current board
 * @return  Score of the current board according to the Hash table. if not found, return NO_HASH_FOUND value
 */
static inline int readHashEntry(int alpha, int beta,int* bestMove, int depth) {
    // retrieving entry that matches the current hash
    TTEntry *hashEntry = &hashTable[hashKey % hashEntries];

    if (hashEntry->hashKey == hashKey) {
        //only if hash matches
        if (hashEntry->depth >= depth) {
            //only if depth is deep enough in the found entry

            int score = hashEntry->score;

            // fixing score to consider faster mate with ply
            if (score < -MATE_SCORE) {
                score += ply;
            }

            if (score > MATE_SCORE) {
                score -= ply;
            }

            if (hashEntry->flag == PV) {
                // return PV node score
                return score;
            }

            if ((hashEntry->flag == ALPHA) && (score <= alpha)) {
                // fail-low node score
                return alpha;
            }

            if ((hashEntry->flag == BETA) && (score >= beta)) {
                // fail-high node score
                return beta;
            }

        }
        *bestMove = hashEntry->bestMove;
    }

    // if hash entry doesn't exist
    return NO_HASH_FOUND;
}

/**
 * Write Hash Entry of current board state
 * @param score Integer represent the score of the current board
 * @param bestMove Integer represents the best move found so far
 * @param depth  depth of current board
 * @param hashFlag Integer represent the beta score of the current situation
 */
static inline void writeHashEntry(int score, int bestMove ,int depth, int hashFlag) {
    // getting current
    TTEntry *hashEntry = &hashTable[hashKey % hashEntries];

    // fixing score to consider faster mate with ply
    if (score < -MATE_SCORE) {
        score -= ply;
    }

    if (score > MATE_SCORE) {
        score += ply;
    }

    // write hash entry data
    hashEntry->hashKey = hashKey;
    hashEntry->score = score;
    hashEntry->flag = hashFlag;
    hashEntry->depth = depth;
    hashEntry->bestMove = bestMove;
}


#endif //NISSIMENGINECPP_TRANSPOSITIONTABLE_H
