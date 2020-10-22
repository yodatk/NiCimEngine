/**
 * In charge of generating random numbers for piece movement and hash keys
 */

#ifndef NISSIMENGINECPP_RANDOMNUBERS_H
#define NISSIMENGINECPP_RANDOMNUBERS_H

#include "bitboard.h"


/**
 * Initial Random number to create Random numbers from
 */
extern unsigned int randomState;

/**
 * Generate Random Number of 32 bits
 * @return 32 bit random number
 */
unsigned int getRandomU32Number();

/**
 * Generate Random Number of 64 bits
 * @return 64 bit random number
 */
U64 getRandomU64Number();



#endif //NISSIMENGINECPP_RANDOMNUBERS_H
