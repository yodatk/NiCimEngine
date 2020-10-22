/**
 * In charge of hashing board current situation
 */

#ifndef NISSIMENGINECPP_ZORBIESTKEYS_H
#define NISSIMENGINECPP_ZORBIESTKEYS_H

#include "board.h"

#include "Randomnubers.h"


#include "EnumsAndConstans.h"


/**
 * Hash keys for pieces [piece][square]
 */
extern U64 pieceKeys[12][64];

/**
 * Hash keys for enpassant [square]
 */
extern U64 enpassantKeys[64];

/**
 * Hash keys for castling [square]
 */
extern U64 castleKeys[16];

/**
 * Hash key for side (white = hash off, black = hash on)
 */
extern U64 sideKey;


/**
 * Generating hash keys to describe situation on the board
 */
void initHashKeys();

/**
 * Generate hash key for the current situation on the board
 * @return U64 almost unique hash key
 */
U64 generate_hash_key();


#endif //NISSIMENGINECPP_ZORBIESTKEYS_H
