/**
 * all the functions that are manipulating bitboard data, encoding bitboard data or extracting it
 */
#include <cstdio>

#ifndef NISSIMENGINECPP_BITBOARD_H
#define NISSIMENGINECPP_BITBOARD_H

/**
 * Shortcut for "unsigned long long" will be used as bitboardsPieces
 */
#define U64 unsigned long long

/**
 * setting given square on given bit board to '1'
 */
#define setBitOn(bitboard, square) ((bitboard) |= (1ULL << (square)))

/**
 * getting the value of the given sqaure on the given bitboard
 */
#define getBit(bitboard, square) ((bitboard) & (1ULL << (square)))

/**
 * setting the given square on the given bitboard to '0'
 */
#define setBitOff(bitboard, square) ((bitboard) &= ~(1ULL << (square)))

/**
 * count all the bits that are '1' in the given bitboard
 * @param bitboard U64 number to count bits
 * @return number of bits that their value is '1'
 */
static inline int countBits(U64 bitboard) {
    int count = 0;
    // as long as the bitboard is not 0ULL, increment counter and remove a bit
    while (bitboard) {

        count++;
        // removing '1' bit
        bitboard &= bitboard - 1;
    }
    return count;
}

/**
 * get least significant bit index that it's value is '1' in the given bitboard
 * @param bitboard bitboard to get the index from
 * @return index of the first bit which is '1' in the given bitboard. if there is no '1' bit, return -1
 */
static inline int getLSBIndex(U64 bitboard) {
    // only if bitboard is not 0ULL
    if (bitboard) {
        // count trailing bits before LSB
        return countBits((bitboard & -bitboard) - 1);
    } else
        // there is no '1' in the given bitboard -> invalid input
        return -1;
}

/**
 * print given bitboard to visual form
 * @param bitboard bitboard to print
 */
void printBitboard(U64 bitboard);

#endif //NISSIMENGINECPP_BITBOARD_H


