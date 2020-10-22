#include "evaluation.h"


/**
 * 2D array to get piece value [game phase][piece]
 */
const int materialScore[2][12] =
        {
                // opening material score
                {82, 337, 365, 477, 1025, 12000, -82, -337, -365, -477, -1025, -12000},

                // endgame material score
                {94, 281, 297, 512, 936,  12000, -94, -281, -297, -512, -936,  -12000}
        };

/**
 * Constants to determine if the current board is in opening, middle game or end game
 */
const int openingPhaseScore = 6192;
const int endgamePhaseScore = 518;


/**
 * Table to calculate score according to piece position [game phase][piece][square]
 */
const int positionalScore[2][6][64] =

        // opening positional piece scores //
        {
                {
                        //pawn
                        {0, 0, 0, 0, 0, 0, 0, 0,
                                98,  134, 61,  95,  68,  126, 34,  -11,
                                -6, 7,   26, 31, 65, 56, 25, -20,
                                -14, 13, 6,  21, 23, 12, 17, -23,
                                -27, -2, -5, 12, 17, 6,  10, -25,
                                -26, -4, -4, -10, 3, 3,  33, -12,
                                -35, -1, -20, -23, -15, 24, 38, -22,
                                0, 0, 0, 0, 0, 0, 0, 0},

                        // knight
                        {-167, -89, -34, -49, 61,  -97, -15, -107,
                                -73, -41, 72,  36, 23, 62,  7,   -17,
                                -47, 60,  37, 65, 84, 129, 73,  44,
                                -9,  17, 19, 53, 37, 69, 18, 22,
                                -13, 4,  16, 13, 28, 19, 21, -8,
                                -23, -9, 12, 10, 19, 17, 25,  -16,
                                -29, -53, -12, -3, -1, 18,  -14, -19,
                                -105, -21, -58, -33, -17, -28, -19, -23},

                        // bishop
                        {-29, 4,   -82, -37, -25, -42, 7,   -8,
                                -26, 16, -18, -13, 30, 59,  18, -47,
                                -16, 37, 43, 40, 35, 50, 37, -2,
                                -4, 5, 19, 50, 37, 37, 7, -2,
                                -6, 13, 13, 26, 34, 12, 10, 4,
                                0,   15, 15, 15, 14, 27, 18, 10,
                                4,   15,  16, 0,  7, 21, 33,  1,
                                -33, -3, -14, -21, -13, -12, -39, -21},

                        // rook
                        {32, 42, 32, 51, 63, 9,  31, 43,
                                27, 32, 58, 62, 80, 67, 26, 44,
                                -5, 19, 26, 36, 17, 45, 61, 16,
                                -24, -11, 7,  26, 24, 35, -8, -20,
                                -36, -26, -12, -1, 9,  -7, 6,  -23,
                                -45, -25, -16, -17, 3,  0,   -5, -33,
                                -44, -16, -20, -9, -1, 11, -6,  -71,
                                -19, -13, 1, 17, 16, 7,   -37, -26},

                        // queen
                        {-28, 0,  29, 12, 59, 44, 43, 45,
                                -24, -39, -5, 1,  -16, 57, 28, 54,
                                -13, -17, 7, 8,  29, 56, 47, 57,
                                -27, -27, -16, -16, -1, 17, -2, 1,
                                -9,  -26, -9, -10, -2, -4, 3,  -3,
                                -14, 2,   -11, -2, -5, 2,  14, 5,
                                -35, -8,  11,  2,   8,   15,  -3,  1,
                                -1,  -18, -9,  10,  -15, -25, -31, -50},

                        // king
                        {-65, 23,  16,  -15, -56, -34, 2, 13,
                                29,  -1, -20, -7, -8, -4, -38, -29,
                                -9, 24, 2,  -16, -20, 6,  22, -22,
                                -17, -20, -12, -27, -30, -25, -14, -36,
                                -49, -1, -27, -39, -46, -44, -33, -51,
                                -14, -14, -22, -46, -44, -30, -15, -27,
                                1,   7,   -8, -64, -43, -16, 9,  8,
                                -15, 36,  12,  -54, 8,   -28, 24,  14},
                },

                // Endgame positional piece scores //
                {
                        //pawn
                        {0, 0, 0, 0, 0, 0, 0, 0,
                                178, 173, 158, 134, 147, 132, 165, 187,
                                94, 100, 85, 67, 56, 53, 82, 84,
                                32,  24, 13, 5,  -2, 4,  17, 17,
                                13,  9,  -3, -7, -7, -8, 3,  -1,
                                4,   7,  -6, 1,   0, -5, -1, -8,
                                13,  8,  8,   10,  13,  0,  2,  -7,
                                0, 0, 0, 0, 0, 0, 0, 0},

                        // knight
                        {-58,  -38, -13, -28, -31, -27, -63, -99,
                                -25, -8,  -25, -2, -9, -25, -24, -52,
                                -24, -20, 10, 9,  -1, -9,  -19, -41,
                                -17, 3,  22, 22, 22, 11, 8,  -18,
                                -18, -6, 16, 25, 16, 17, 4,  -18,
                                -23, -3, -1, 15, 10, -3, -20, -22,
                                -42, -20, -10, -5, -2, -20, -23, -44,
                                -29,  -51, -23, -15, -22, -18, -50, -64},

                        // bishop
                        {-14, -21, -11, -8,  -7,  -9,  -17, -24,
                                -8,  -4, 7,   -12, -3, -13, -4, -14,
                                2,   -8, 0,  -1, -2, 6,  0,  4,
                                -3, 9, 12, 9,  14, 10, 3, 2,
                                -6, 3,  13, 19, 7,  10, -3, -9,
                                -12, -3, 8,  10, 13, 3,  -7, -15,
                                -14, -18, -7, -1, 4, -9, -15, -27,
                                -23, -9, -23, -5,  -9,  -16, -5,  -17},

                        // rook
                        {13, 10, 18, 15, 12, 12, 8,  5,
                                11, 13, 13, 11, -3, 3,  8,  3,
                                7,  7,  7,  5,  4,  -3, -5, -3,
                                4,   3,   13, 1,  2,  1,  -1, 2,
                                3,   5,   8,   4,  -5, -6, -8, -11,
                                -4,  0,   -5,  -1,  -7, -12, -8, -16,
                                -6,  -6,  0,   2,  -9, -9, -11, -3,
                                -9,  2,   3, -1, -5, -13, 4,   -20},

                        // queen
                        {-9,  22, 22, 27, 27, 19, 10, 20,
                                -17, 20,  32, 41, 58,  25, 30, 0,
                                -20, 6,   9, 49, 47, 35, 19, 9,
                                3,   22,  24,  45,  57, 40, 57, 36,
                                -18, 28,  19, 47,  31, 34, 39, 23,
                                -16, -27, 15,  6,  9,  17, 10, 5,
                                -22, -23, -30, -16, -16, -23, -36, -32,
                                -33, -28, -22, -43, -5,  -32, -20, -41},

                        // king
                        {-74, -35, -18, -18, -11, 15,  4, -17,
                                -12, 17, 14,  17, 17, 38, 23,  11,
                                10, 17, 23, 15,  20,  45, 44, 13,
                                -8,  22,  24,  27,  26,  33,  26,  3,
                                -18, -4, 21,  24,  27,  23,  9,   -11,
                                -19, -3,  11,  21,  23,  16,  7,   -9,
                                -27, -11, 4,  13,  14,  4,   -5, -17,
                                -53, -34, -21, -11, -28, -14, -24, -43}
                }
        };
/**
 * mirror positional score tables for opposite side, for black side easy evaluation
 */
const int mirrorScore[128] =
        {
                A1, B1, C1, D1, E1, F1, G1, H1,
                A2, B2, C2, D2, E2, F2, G2, H2,
                A3, B3, C3, D3, E3, F3, G3, H3,
                A4, B4, C4, D4, E4, F4, G4, H4,
                A5, B5, C5, D5, E5, F5, G5, H5,
                A6, B6, C6, D6, E6, F6, G6, H6,
                A7, B7, C7, D7, E7, F7, G7, H7,
                A8, B8, C8, D8, E8, F8, G8, H8
        };

/**
 * File masks for each square
 */
U64 fileMasks[64];

/**
 * Rank masks for each square
 */
U64 rankMasks[64];

/**
 * Masks for isolated pawn evaluation [square]
 */
U64 isolatedMasks[64];

/**
 * Masks for White Passed Pawns[square]
 */
U64 whitePassedMasks[64];

/**
 * Masks for Black Passed Pawns[square]
 */
U64 blackPassedMasks[64];


/**
 * get rank according to square array
 */
const int getRank[64] =
        {
                7, 7, 7, 7, 7, 7, 7, 7,
                6, 6, 6, 6, 6, 6, 6, 6,
                5, 5, 5, 5, 5, 5, 5, 5,
                4, 4, 4, 4, 4, 4, 4, 4,
                3, 3, 3, 3, 3, 3, 3, 3,
                2, 2, 2, 2, 2, 2, 2, 2,
                1, 1, 1, 1, 1, 1, 1, 1,
                0, 0, 0, 0, 0, 0, 0, 0
        };


/**
 * Setting Mask for given File or given Rank
 * @param fileNumber int represent the file to create mask for. if none, is -1
 * @param rankNumber int represent the rank to create mask for. if none, is -1
 * @return Unsigned long long which is the output mask for the given rank \ file
 */
U64 setFileRankMask(int fileNumber, int rankNumber) {
    U64 mask = 0ULL;


    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            int square = rank * 8 + file;

            if (fileNumber != -1) {
                // handaling files
                if (file == fileNumber) {
                    mask |= setBitOn(mask, square);
                }

            } else if (rankNumber != -1) {
                // handaling ranks
                if (rank == rankNumber) {
                    mask |= setBitOn(mask, square);
                }
            }
        }
    }
    return mask;
}

/**
 * init all masks for the evaluation process
 */
void initEvaluationMasks() {
    // ======= Init file masks ======= /

    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            int square = rank * 8 + file;
            fileMasks[square] |= setFileRankMask(file, -1);
        }
    }

    // ======= Init rank masks ======= /


    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            int square = rank * 8 + file;
            rankMasks[square] |= setFileRankMask(-1, rank);
        }
    }

    // ======= Init isolated masks ======= /


    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            int square = rank * 8 + file;
            isolatedMasks[square] |= setFileRankMask(file - 1, -1);
            isolatedMasks[square] |= setFileRankMask(file + 1, -1);
        }
    }

    // ======= Init White passed masks ======= /

    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            int square = rank * 8 + file;
            whitePassedMasks[square] |= setFileRankMask(file - 1, -1);
            whitePassedMasks[square] |= setFileRankMask(file, -1);
            whitePassedMasks[square] |= setFileRankMask(file + 1, -1);

            for (int i = 0; i < (8 - rank); i++) {
                whitePassedMasks[square] &= ~rankMasks[(7 - i) * 8 + file];
            }

        }
    }

    // ======= Init Black passed masks ======= /

    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            int square = rank * 8 + file;

            blackPassedMasks[square] |= setFileRankMask(file - 1, -1);
            blackPassedMasks[square] |= setFileRankMask(file, -1);
            blackPassedMasks[square] |= setFileRankMask(file + 1, -1);

            for (int i = 0; i < rank + 1; i++) {
                blackPassedMasks[square] &= ~rankMasks[i * 8 + file];
            }

        }
    }
}


