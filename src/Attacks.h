/**
 * All the attacks movement calculation for slider pieces, and non slider pieces
 */
#ifndef NISSIMENGINECPP_ATTACKS_H
#define NISSIMENGINECPP_ATTACKS_H

#include "Bitboard.h"
#include "EnumsAndConstants.h"

/**
 * bitboard to represents all squares that are not on A file
 */
extern const U64 notAFile;

/**
 * bitboard to represents all squares that are not on H file
 */
extern const U64 notHFile;

/**
 * bitboard to represents all squares that are not on HG file
 */
extern const U64 notHGFile;

/**
 * bitboard to represents all squares that are not on AB file
 */
extern const U64 notABFile;

/**
 * All relevant attacking \ movement squares for bishop according to given square
 */
extern const int bishopRelevantBits[64];

/**
 * All relevant attacking \ movement squares for rook according to given square
 */
extern const int rookRelevantBits[64];

/**
 * Magic numbers array for rook for calculating rooks move on  the fly
 */
extern U64 rookMagicNumbers[64];
/**
 * Magic numbers array for rook for calculating bishop move on  the fly
 */
extern U64 bishopMagicNumbers[64];

/**
 * pre calculated attacks of pawns according to pawn side, and pawn location [side] [square]
 */
extern U64 pawnAttacks[2][64];

/**
 * pre calculated attacks of knights according to location [square]
 */
extern U64 knightAttacks[64];

/**
 * pre calculated attacks of king according to location [square]
 */
extern U64 kingAttacks[64];

/**
 * all bishop masks according to location [square]
 */
extern U64 bishopMasks[64];

/**
 * all rook masks according to location [square]
 */
extern U64 rookMasks[64];

/**
 * Pre calculated attacks for bishop according to location, and occupancies [square][occupancies]
 */
extern U64 bishopAttacks[64][512];

/**
 * Pre calculated attacks for rook according to location, and occupancies [square][occupancies]
 */
extern U64 rookAttacks[64][4096];

/**
 * generate all masks for pawns
 * @param side color of the pawn (0 = WHITE, 1=BLACK)
 * @param square integer represents the square of the wanted mask
 * @return bitboard that represents the wanted mask
 */
extern U64 maskPawnAttacks(int side, int square);

/**
 * generate all masks for knights
 * @param square integer represents the square of the wanted mask
 * @return bitboard that represents the wanted mask
 */
extern U64 maskKnightAttacks(int square);

/**
 * generate all masks for king
 * @param square integer represents the square of the wanted mask
 * @return bitboard that represents the wanted mask
 */
extern U64 maskKingAttacks(int square);
/**
 * generate all masks for bishop
 * @param square integer represents the square of the wanted mask
 * @return bitboard that represents the wanted mask
 */
U64 maskBishopAttacks(int square);


/**
 * generate all masks for rooks
 * @param square integer represents the square of the wanted mask
 * @return bitboard that represents the wanted mask
 */
U64 maskRookAttacks(int square);


/**
 * Calculate attacks of bishop on the fly according to location of the bishop and current occupancy
 * @param square location of the bishop
 * @param block occupencies of both pieces of the board
 * @return bitboard represents all the possible moves for the bishop on the given square
 */
U64 bishopAttacksOnTheFly(int square, U64 block);

/**
 * Calculate attacks of rook on the fly according to location of the bishop and current occupancy
 * @param square location of the rook
 * @param block occupencies of both pieces of the board
 * @return bitboard represents all the possible moves for the rook on the given square
 */
U64 rookAttacksOnTheFly(int square, U64 block);

/**
 * Init all the moves for the non sliding pieces
 */
void initLeapersAttacks();

/**
 * Setting occupancies
 * @param index index of current occupency
 * @param bitsInMask bits in the current mask
 * @param attackMask bitboard of current attack mask
 * @return bitboard represents the wanted occupancy
 */
U64 setOccupancy(int index, int bitsInMask, U64 attackMask);

/**
 * Init all the possible moves for slider pieces for rook or bishop
 * @param bishop flag to determine which piece to generate attack to (1 -> bishop, 0 -> rook)
 */
void initSlidersAttacks(int bishop);

/**
 * get all bishop attack for given square and occupancy
 * @param square location of the bishop on the board
 * @param occupancy occupancy of both colors on the board
 * @return bitboard represents all the possible moves of the bishop
 */
static inline U64 getBishopAttacks(int square, U64 occupancy) {
    // get bishop attacks assuming current board occupancy
    occupancy &= bishopMasks[square];
    occupancy *= bishopMagicNumbers[square];
    occupancy >>= 64 - bishopRelevantBits[square];
    return bishopAttacks[square][occupancy];
}

/**
 * get all rook attack for given square and occupancy
 * @param square location of the rook on the board
 * @param occupancy occupancy of both colors on the board
 * @return bitboard represents all the possible moves of the rook
 */
static inline U64 getRookAttacks(int square, U64 occupancy) {
    // get rook attacks assuming current board occupancy
    occupancy &= rookMasks[square];
    occupancy *= rookMagicNumbers[square];
    occupancy >>= 64 - rookRelevantBits[square];
    return rookAttacks[square][occupancy];
}


/**
 * get all queen attack for given location and occupancy
 * @param square location of the queen on the board
 * @param occupancy current occupancy of Both pieces colors on the board
 * @return bitboard represents all the possible moves of the queen
 */
static inline U64 getQueenAttacks(int square, U64 occupancy) {
    return (getBishopAttacks(square, occupancy) | getRookAttacks(square, occupancy));
}
#endif //NISSIMENGINECPP_ATTACKS_H
