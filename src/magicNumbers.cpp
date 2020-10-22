#include "magicNumbers.h"
#include <cstdio>
#include <cstring>


/**
 * find magic numbers according to random state, and given params
 * @param square integer represent the square of the magic number
 * @param relevantBits int represents the relavant bits for that square
 * @param bishop int flag to tell if it's a magic number for rook, or for a bishop (0->Rook,!0 -> Bishop)
 * @return unisnged long long number to be magic number for sliding pieces movements
 */
U64 findMagicNumber(int square, int relevantBits, int bishop) {
    // init data for search
    U64 occupancies[4096];
    U64 attacks[4096];
    U64 usedAttacks[4096];
    U64 attackMask = bishop ? maskBishopAttacks(square) : maskRookAttacks(square);

    // init occupancy indicies
    int occupancyIndicies = 1 << relevantBits;

    // init attacks and occupencies
    for (int index = 0; index < occupancyIndicies; index++) {
        occupancies[index] = setOccupancy(index, relevantBits, attackMask);
        attacks[index] = bishop ? bishopAttacksOnTheFly(square, occupancies[index]) : rookAttacksOnTheFly(
                square, occupancies[index]);
    }

    // loop for finding single magic number
    for (int randomCount = 0; randomCount < 100000000; randomCount++) {
        // generate magic number candidate
        U64 magicNumber = generate_magic_number();

        if (countBits((attackMask * magicNumber) & 0xFF00000000000000) < 6) {
            // invalid magic number -> try again
            continue;
        }


        memset(usedAttacks, 0ULL, sizeof(usedAttacks));

        // init index & fail flag
        int index, fail;

        // test magic index loop
        for (index = 0, fail = 0; !fail && index < occupancyIndicies; index++) {
            // creating magic index
            int magic_index = (int) ((occupancies[index] * magicNumber) >> (64 - relevantBits));


            if (usedAttacks[magic_index] == 0ULL) {
                // magic number fit -> init used attacks
                usedAttacks[magic_index] = attacks[index];
            } else if (usedAttacks[magic_index] != attacks[index]) {
                // magic index doesn't work -> turn fail flag
                fail = 1;
            }

        }

        if (!fail) {
            // found magic number
            return magicNumber;
        }

    }

    // didn't found magic number -> something went wrong
    printf("  Magic number fails!\n");
    return 0ULL;
}

/**
 * init all needed Magic numbers for rook and bishop
 */
void init_magic_numbers() {
    // creating magic number for rook
    for (int square = 0; square < 64; square++) {
        rookMagicNumbers[square] = findMagicNumber(square, rookRelevantBits[square], rook);
    }
    // creating magic number for bishop
    for (int square = 0; square < 64; square++) {
        bishopMagicNumbers[square] = findMagicNumber(square, bishopRelevantBits[square], bishop);
    }


}


/**
 * generate a random number as candidate to be a magic number
 * @return unsigned long long number to be candidate magic number
 */
U64 generate_magic_number() {
    return getRandomU64Number() & getRandomU64Number() & getRandomU64Number();
}
