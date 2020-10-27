

#ifndef NICIMENGINE_POLYBOOK_H
#define NICIMENGINE_POLYBOOK_H

#include "PolyKeys.h"

#define NAME_OF_BOOK "performance.bin"

typedef struct {
    U64 key;
    unsigned short move;
    unsigned short weight;
    unsigned int learn;
} BookEntry;

/**
 * number of entries in the book
 */
extern long NUM_OF_ENTRIES;

/**
 * pointer to the current entry
 */
extern BookEntry* entries;

/**
 * init Entries in book
 */
int initPolyBook();

/**
 * free memory of book
 */
void cleanPolyBook();

extern const int PolyTypeOfPiece[12];

extern int reversedSquares[64];

U64 polyKeyFromBoard();


#endif //NICIMENGINE_POLYBOOK_H
