
#include "TranspositionTable.h"

/**
 * Total number of entries in Hash Table
 */
int hashEntries = 0;


/**
 * HashTable / Transposition Table
 */
TTEntry *hashTable = nullptr;

/**
 * Clearing Hash Table from all data, and free all related allocated data
 */
void clearHashTable() {
    // init hash table entry pointer
    TTEntry *hash_entry;

    // loop over TT elements
    for (hash_entry = hashTable; hash_entry < hashTable + hashEntries; hash_entry++) {
        // reset TT inner fields
        hash_entry->hashKey = 0;
        hash_entry->depth = 0;
        hash_entry->flag = 0;
        hash_entry->score = 0;
    }
}

/**
 * Initializing hash table
 * @param mb max size of allocated data for the table in MB
 */
void initHashTable(int mb) {

    int hash_size = 0x100000 * mb;

    // calculating number of hash entries
    hashEntries = hash_size / sizeof(TTEntry);

    if (hashTable != nullptr) {
        // if memory is allocated -> free it
        printf("    Clearing hash memory...\n");
        free(hashTable);
    }

    // allocate memory for hash table
    hashTable = (TTEntry *) malloc(hashEntries * sizeof(TTEntry));


    if (hashTable == nullptr) {
        // if allocation has failed -> try again with smaller size
        printf("    Couldn't allocate memory for hash table, trying %dMB...", mb / 2);
        initHashTable(mb / 2);
    } else {
        // allocation succeeded -> initilize table to an empty entries
        clearHashTable();

        printf("    Hash table is initialized with %d entries\n", hashEntries);
    }
}

