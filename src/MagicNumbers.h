
#ifndef NISSIMENGINECPP_MAGICNUMBERS_H
#define NISSIMENGINECPP_MAGICNUMBERS_H

#include "Attacks.h"
#include "RandomNumbers.h"

/**
 * find magic numbers according to random state, and given params
 * @param square integer represent the square of the magic number
 * @param relevantBits int represents the relavant bits for that square
 * @param bishop int flag to tell if it's a magic number for rook, or for a bishop (0->Rook,!0 -> Bishop)
 * @return unsigned long long number to be magic number for sliding pieces movements
 */
U64 findMagicNumber(int square, int relevantBits, int bishop);


/**
 * init all needed Magic numbers for rook and bishop
 */
void init_magic_numbers();

/**
 * generate a random number as candidate to be a magic number
 * @return unsigned long long number to be candidate magic number
 */
U64 generate_magic_number();

#endif //NISSIMENGINECPP_MAGICNUMBERS_H
