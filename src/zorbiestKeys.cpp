
#include "zorbiestKeys.h"



/**
 * Hash keys for pieces [piece][square]
 */
U64 pieceKeys[12][64];

/**
 * Hash keys for enpassant [square]
 */
U64 enpassantKeys[64];

/**
 * Hash keys for castling [square]
 */
U64 castleKeys[16];

/**
 * Hash key for side (white = hash off, black = hash on)
 */
U64 sideKey;


/**
 * Generating hash keys to describe situation on the board
 */
void initHashKeys() {
    // update pseudo random number state
    randomState = 1804289383;


    for (int piece = P; piece <= k; piece++) {

        for (int square = 0; square < 64; square++)
            // init hash key for current piece on current square
            pieceKeys[piece][square] = getRandomU64Number();
    }

    for (int square = 0; square < 64; square++)
        // init hash keys for enpassant
        enpassantKeys[square] = getRandomU64Number();


    for (int index = 0; index < 16; index++)
        // init current castling rights hash key
        castleKeys[index] = getRandomU64Number();

    // init hash key for side
    sideKey = getRandomU64Number();
}

/**
 * Generate hash key for the current situation on the board
 * @return U64 almost unique hash key
 */
U64 generate_hash_key() {
    // final hash key
    U64 outputKey = 0ULL;

    U64 bitboard;

    for (int piece = P; piece <= k; piece++) {
        // init piece bitboard copy
        bitboard = bitboards[piece];

        // for each piece from current piece type
        while (bitboard) {
            // get piece location square
            int square = getLSBIndex(bitboard);

            // hash piece into output key
            outputKey ^= pieceKeys[piece][square];

            // remove piece from copy bitboard
            setBitOff(bitboard, square);
        }
    }


    if (enpassant != NO_SQUARE) {
        // if enpassant square is on board -> hash it to output key
        outputKey ^= enpassantKeys[enpassant];
    }


    // hash castling rights
    outputKey ^= castleKeys[castle];

    // hash the side only if BLACK is to move
    if (side == BLACK) {
        outputKey ^= sideKey;
    }
    return outputKey;
}
