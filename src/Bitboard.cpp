
#include "Bitboard.h"


/**
 * print given bitboard to visual form
 * @param bitboard bitboard to print
 */
void printBitboard(U64 bitboard) {

    printf("\n");
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            int square = rank * 8 + file;
            if (!file) {
                printf("  %d ", 8 - rank);
            }
            // printing 1 if bit is no 0
            printf(" %d", getBit(bitboard, square) ? 1 : 0);
        }
        printf("\n");
    }
    printf("\n     A B C D E F G H\n\n");
    printf("     Bitboard: %llu\n\n", bitboard);
}