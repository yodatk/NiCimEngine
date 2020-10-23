

#include "RandomNumbers.h"

/**
 * Initial Random number to create Random numbers from
 */
unsigned int randomState = 1804289383;

/**
 * Generate Random Number of 32 bits
 * @return 32 bit random number
 */
unsigned int getRandomU32Number() {

    unsigned int number = randomState;

    // XOR shift algorithm
    number ^= number << 13;
    number ^= number >> 17;
    number ^= number << 5;

    // updating state and returning number
    randomState = number;
    return number;
}

/**
 * Generate Random Number of 64 bits
 * @return 64 bit random number
 */
U64 getRandomU64Number() {

    U64 n1, n2, n3, n4;

    // init random numbers slicing 16 bits from MS1B side
    n1 = (U64) (getRandomU32Number()) & 0xFFFF;
    n2 = (U64) (getRandomU32Number()) & 0xFFFF;
    n3 = (U64) (getRandomU32Number()) & 0xFFFF;
    n4 = (U64) (getRandomU32Number()) & 0xFFFF;

    // return random number
    return n1 | (n2 << 16) | (n3 << 32) | (n4 << 48);
}




