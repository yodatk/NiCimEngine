

#ifndef NICIMENGINE_POLYBOOK_H
#define NICIMENGINE_POLYBOOK_H

#include "PolyKeys.h"

#define NAME_OF_BOOK "performance.bin"

/**
 * Struct for a move entry from opening book
 */
typedef struct {
    U64 key;
    unsigned short move;
    unsigned short weight;
    unsigned int learn;
} BookEntry;

/**
 * Array to convert NiCim pieces type to polyglot pieces
 */
extern const int PolyTypeOfPiece[12];

/**
 * Array to convert NiCim Squares to polyglot squares
 */
extern int reversedSquares[64];

/**
 * boolean flag to determine if to use opening book or not IN CURRENT MOVE
 */
extern int IS_USING_BOOK;

/**
 * boolean flag to detemine if to use opening book or not IN ALL GAMES
 */
extern int IS_USING_BOOK_SUDO;

/**
 * number of entries in the book
 */
extern long NUM_OF_ENTRIES;

/**
 * pointer to the current entry
 */
extern BookEntry *entries;

/**
 * init Entries in book into memory
 */
int initPolyBook();

/**
 * free memory of book
 */
void cleanPolyBook();


/**
 * generate poly hashkey from current board
 * @return U64 poly hashed key
 */
U64 polyKeyFromBoard();

/**
 * get random move from opening book for current situation. if no move available, return 0
 * @return int represents an encoded move from book. if not available, return 0
 */
int getBookMove();

//void listBookMoves(U64 move);


#endif //NICIMENGINE_POLYBOOK_H
