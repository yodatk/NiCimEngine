/**
 * All the attacks movement calculation for slider pieces, and non slider pieces
 */
#include "Attacks.h"


/**
 * bitboard to represents all squares that are not on A file
 */
const U64 notAFile = 18374403900871474942ULL;

/**
 * bitboard to represents all squares that are not on H file
 */
const U64 notHFile = 9187201950435737471ULL;

/**
 * bitboard to represents all squares that are not on HG file
 */
const U64 notHGFile = 4557430888798830399ULL;

/**
 * bitboard to represents all squares that are not on AB file
 */
const U64 notABFile = 18229723555195321596ULL;

/**
 * All relevant attacking \ movement squares for bishop according to given square
 */
const int bishopRelevantBits[64] = {
        6, 5, 5, 5, 5, 5, 5, 6,
        5, 5, 5, 5, 5, 5, 5, 5,
        5, 5, 7, 7, 7, 7, 5, 5,
        5, 5, 7, 9, 9, 7, 5, 5,
        5, 5, 7, 9, 9, 7, 5, 5,
        5, 5, 7, 7, 7, 7, 5, 5,
        5, 5, 5, 5, 5, 5, 5, 5,
        6, 5, 5, 5, 5, 5, 5, 6};

/**
 * All relevant attacking \ movement squares for rook according to given square
 */
const int rookRelevantBits[64] = {
        12, 11, 11, 11, 11, 11, 11, 12,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        12, 11, 11, 11, 11, 11, 11, 12};

/**
 * Magic numbers array for rook for calculating rooks move on  the fly
 */
U64 rookMagicNumbers[64] = {
        0x8a80104000800020ULL,
        0x140002000100040ULL,
        0x2801880a0017001ULL,
        0x100081001000420ULL,
        0x200020010080420ULL,
        0x3001c0002010008ULL,
        0x8480008002000100ULL,
        0x2080088004402900ULL,
        0x800098204000ULL,
        0x2024401000200040ULL,
        0x100802000801000ULL,
        0x120800800801000ULL,
        0x208808088000400ULL,
        0x2802200800400ULL,
        0x2200800100020080ULL,
        0x801000060821100ULL,
        0x80044006422000ULL,
        0x100808020004000ULL,
        0x12108a0010204200ULL,
        0x140848010000802ULL,
        0x481828014002800ULL,
        0x8094004002004100ULL,
        0x4010040010010802ULL,
        0x20008806104ULL,
        0x100400080208000ULL,
        0x2040002120081000ULL,
        0x21200680100081ULL,
        0x20100080080080ULL,
        0x2000a00200410ULL,
        0x20080800400ULL,
        0x80088400100102ULL,
        0x80004600042881ULL,
        0x4040008040800020ULL,
        0x440003000200801ULL,
        0x4200011004500ULL,
        0x188020010100100ULL,
        0x14800401802800ULL,
        0x2080040080800200ULL,
        0x124080204001001ULL,
        0x200046502000484ULL,
        0x480400080088020ULL,
        0x1000422010034000ULL,
        0x30200100110040ULL,
        0x100021010009ULL,
        0x2002080100110004ULL,
        0x202008004008002ULL,
        0x20020004010100ULL,
        0x2048440040820001ULL,
        0x101002200408200ULL,
        0x40802000401080ULL,
        0x4008142004410100ULL,
        0x2060820c0120200ULL,
        0x1001004080100ULL,
        0x20c020080040080ULL,
        0x2935610830022400ULL,
        0x44440041009200ULL,
        0x280001040802101ULL,
        0x2100190040002085ULL,
        0x80c0084100102001ULL,
        0x4024081001000421ULL,
        0x20030a0244872ULL,
        0x12001008414402ULL,
        0x2006104900a0804ULL,
        0x1004081002402ULL};

/**
 * Magic numbers array for rook for calculating bishop move on  the fly
 */
U64 bishopMagicNumbers[64] = {
        0x40040844404084ULL,
        0x2004208a004208ULL,
        0x10190041080202ULL,
        0x108060845042010ULL,
        0x581104180800210ULL,
        0x2112080446200010ULL,
        0x1080820820060210ULL,
        0x3c0808410220200ULL,
        0x4050404440404ULL,
        0x21001420088ULL,
        0x24d0080801082102ULL,
        0x1020a0a020400ULL,
        0x40308200402ULL,
        0x4011002100800ULL,
        0x401484104104005ULL,
        0x801010402020200ULL,
        0x400210c3880100ULL,
        0x404022024108200ULL,
        0x810018200204102ULL,
        0x4002801a02003ULL,
        0x85040820080400ULL,
        0x810102c808880400ULL,
        0xe900410884800ULL,
        0x8002020480840102ULL,
        0x220200865090201ULL,
        0x2010100a02021202ULL,
        0x152048408022401ULL,
        0x20080002081110ULL,
        0x4001001021004000ULL,
        0x800040400a011002ULL,
        0xe4004081011002ULL,
        0x1c004001012080ULL,
        0x8004200962a00220ULL,
        0x8422100208500202ULL,
        0x2000402200300c08ULL,
        0x8646020080080080ULL,
        0x80020a0200100808ULL,
        0x2010004880111000ULL,
        0x623000a080011400ULL,
        0x42008c0340209202ULL,
        0x209188240001000ULL,
        0x400408a884001800ULL,
        0x110400a6080400ULL,
        0x1840060a44020800ULL,
        0x90080104000041ULL,
        0x201011000808101ULL,
        0x1a2208080504f080ULL,
        0x8012020600211212ULL,
        0x500861011240000ULL,
        0x180806108200800ULL,
        0x4000020e01040044ULL,
        0x300000261044000aULL,
        0x802241102020002ULL,
        0x20906061210001ULL,
        0x5a84841004010310ULL,
        0x4010801011c04ULL,
        0xa010109502200ULL,
        0x4a02012000ULL,
        0x500201010098b028ULL,
        0x8040002811040900ULL,
        0x28000010020204ULL,
        0x6000020202d0240ULL,
        0x8918844842082200ULL,
        0x4010011029020020ULL};

/**
 * pre calculated attacks of pawns according to pawn side, and pawn location [side] [square]
 */
U64 pawnAttacks[2][64];

/**
 * pre calculated attacks of knights according to location [sqaure]
 */
U64 knightAttacks[64];

/**
 * pre calculated attacks of king according to location [square]
 */
U64 kingAttacks[64];

/**
 * all bishop masks according to location [square]
 */
U64 bishopMasks[64];

/**
 * all rook masks according to location [square]
 */
U64 rookMasks[64];

/**
 * Pre calculated attacks for bishop according to location, and occupancies [square][occupancies]
 */
U64 bishopAttacks[64][512];

/**
 * Pre calculated attacks for rook according to location, and occupancies [square][occupancies]
 */
U64 rookAttacks[64][4096];

/**
 * generate all masks for pawns
 * @param side color of the pawn (0 = WHITE, 1=BLACK)
 * @param square integer represents the square of the wanted mask
 * @return bitboard that represents the wanted mask
 */
U64 maskPawnAttacks(int side, int square) {

    U64 attacks = 0ULL;
    U64 bitboard = 0ULL;
    // setting pawn on bitboard
    setBitOn(bitboard, square);

    // generate pawn attacks according to side
    if (!side) {
        // WHITE pawns
        if ((bitboard >> 7) & notAFile)
            attacks |= (bitboard >> 7);
        if ((bitboard >> 9) & notHFile)
            attacks |= (bitboard >> 9);
    } else {
        // BLACK pawns
        if ((bitboard << 7) & notHFile)
            attacks |= (bitboard << 7);
        if ((bitboard << 9) & notAFile)
            attacks |= (bitboard << 9);
    }

    return attacks;
}

/**
 * generate all masks for knights
 * @param square integer represents the square of the wanted mask
 * @return bitboard that represents the wanted mask
 */
U64 maskKnightAttacks(int square) {

    U64 attacks = 0ULL;
    U64 bitboard = 0ULL;

    // putting knight on bitboard
    setBitOn(bitboard, square);

    // generate knight attacks
    if ((bitboard >> 17) & notHFile) {
        attacks |= (bitboard >> 17);
    }

    if ((bitboard >> 15) & notAFile) {
        attacks |= (bitboard >> 15);
    }

    if ((bitboard >> 10) & notHGFile) {
        attacks |= (bitboard >> 10);
    }

    if ((bitboard >> 6) & notABFile) {
        attacks |= (bitboard >> 6);
    }

    if ((bitboard << 17) & notAFile) {
        attacks |= (bitboard << 17);
    }

    if ((bitboard << 15) & notHFile) {
        attacks |= (bitboard << 15);
    }

    if ((bitboard << 10) & notABFile) {
        attacks |= (bitboard << 10);
    }

    if ((bitboard << 6) & notHGFile) {
        attacks |= (bitboard << 6);
    }
    return attacks;
}

/**
 * generate all masks for king
 * @param square integer represents the square of the wanted mask
 * @return bitboard that represents the wanted mask
 */
U64 maskKingAttacks(int square) {
    U64 attacks = 0ULL;
    U64 bitboard = 0ULL;

    // set king on bitboard
    setBitOn(bitboard, square);

    // generate king attacks
    if (bitboard >> 8) {
        attacks |= (bitboard >> 8);
    }

    if ((bitboard >> 9) & notHFile) {
        attacks |= (bitboard >> 9);
    }

    if ((bitboard >> 7) & notAFile) {
        attacks |= (bitboard >> 7);
    }

    if ((bitboard >> 1) & notHFile) {
        attacks |= (bitboard >> 1);
    }

    if (bitboard << 8) {
        attacks |= (bitboard << 8);
    }

    if ((bitboard << 9) & notAFile) {
        attacks |= (bitboard << 9);
    }

    if ((bitboard << 7) & notHFile) {
        attacks |= (bitboard << 7);
    }

    if ((bitboard << 1) & notAFile) {
        attacks |= (bitboard << 1);
    }
    // return attack map
    return attacks;
}

/**
 * generate all masks for bishop
 * @param square integer represents the square of the wanted mask
 * @return bitboard that represents the wanted mask
 */
U64 maskBishopAttacks(int square) {

    U64 attacks = 0ULL;
    int rank, file, targetRank = square / 8, targetFile = square % 8;

    // mask relevant bishop occupancy bits
    for (rank = targetRank + 1, file = targetFile + 1; rank <= 6 && file <= 6; rank++, file++) {
        attacks |= (1ULL << (rank * 8 + file));
    }

    for (rank = targetRank - 1, file = targetFile + 1; rank >= 1 && file <= 6; rank--, file++) {
        attacks |= (1ULL << (rank * 8 + file));
    }

    for (rank = targetRank + 1, file = targetFile - 1; rank <= 6 && file >= 1; rank++, file--) {
        attacks |= (1ULL << (rank * 8 + file));
    }

    for (rank = targetRank - 1, file = targetFile - 1; rank >= 1 && file >= 1; rank--, file--) {
        attacks |= (1ULL << (rank * 8 + file));
    }

    // return attack map
    return attacks;
}

/**
 * generate all masks for rooks
 * @param square integer represents the square of the wanted mask
 * @return bitboard that represents the wanted mask
 */
U64 maskRookAttacks(int square) {

    U64 attacks = 0ULL;
    int rank, file, targetRank = square / 8, targetFile = square % 8;

    // mask relevant rook occupancy bits
    for (rank = targetRank + 1; rank <= 6; rank++) {
        attacks |= (1ULL << (rank * 8 + targetFile));
    }

    for (rank = targetRank - 1; rank >= 1; rank--) {
        attacks |= (1ULL << (rank * 8 + targetFile));
    }

    for (file = targetFile + 1; file <= 6; file++) {
        attacks |= (1ULL << (targetRank * 8 + file));
    }

    for (file = targetFile - 1; file >= 1; file--) {
        attacks |= (1ULL << (targetRank * 8 + file));
    }
    return attacks;
}

/**
 * calculate attacks of bishop on the fly according to location of the bishop and current occupancy
 * @param square location of the bishop
 * @param block occupencies of both pieces of the board
 * @return bitboard represents all the possible moves for the bishop on the given square
 */
U64 bishopAttacksOnTheFly(int square, U64 block) {
    U64 attacks = 0ULL;

    // init ranks & files
    int rank, file, targetRank = square / 8, targetFile = square % 8;

    // generate bishop atacks
    for (rank = targetRank + 1, file = targetFile + 1; rank <= 7 && file <= 7; rank++, file++) {
        attacks |= (1ULL << (rank * 8 + file));
        if ((1ULL << (rank * 8 + file)) & block) {
            break;
        }

    }

    for (rank = targetRank - 1, file = targetFile + 1; rank >= 0 && file <= 7; rank--, file++) {
        attacks |= (1ULL << (rank * 8 + file));
        if ((1ULL << (rank * 8 + file)) & block) {
            break;
        }
    }

    for (rank = targetRank + 1, file = targetFile - 1; rank <= 7 && file >= 0; rank++, file--) {
        attacks |= (1ULL << (rank * 8 + file));
        if ((1ULL << (rank * 8 + file)) & block) {
            break;
        }
    }
    for (rank = targetRank - 1, file = targetFile - 1; rank >= 0 && file >= 0; rank--, file--) {
        attacks |= (1ULL << (rank * 8 + file));
        if ((1ULL << (rank * 8 + file)) & block) {
            break;
        }
    }
    return attacks;
}

/**
 * calculate attacks of rook on the fly according to location of the bishop and current occupancy
 * @param square location of the rook
 * @param block occupencies of both pieces of the board
 * @return bitboard represents all the possible moves for the rook on the given square
 */
U64 rookAttacksOnTheFly(int square, U64 block) {
    U64 attacks = 0ULL;

    int rank, file, targetRank = square / 8, targetFile = square % 8;

    for (rank = targetRank + 1; rank <= 7; rank++) {
        attacks |= (1ULL << (rank * 8 + targetFile));
        if ((1ULL << (rank * 8 + targetFile)) & block) {
            break;
        }
    }

    for (rank = targetRank - 1; rank >= 0; rank--) {
        attacks |= (1ULL << (rank * 8 + targetFile));
        if ((1ULL << (rank * 8 + targetFile)) & block) {
            break;
        }
    }

    for (file = targetFile + 1; file <= 7; file++) {
        attacks |= (1ULL << (targetRank * 8 + file));
        if ((1ULL << (targetRank * 8 + file)) & block) {
            break;
        }
    }

    for (file = targetFile - 1; file >= 0; file--) {
        attacks |= (1ULL << (targetRank * 8 + file));
        if ((1ULL << (targetRank * 8 + file)) & block) {
            break;
        }
    }
    return attacks;
}

/**
 * Init all the moves for the non sliding pieces
 */
void initLeapersAttacks() {
    for (int square = 0; square < 64; square++) {
        // init pawn attacks
        pawnAttacks[WHITE][square] = maskPawnAttacks(WHITE, square);
        pawnAttacks[BLACK][square] = maskPawnAttacks(BLACK, square);

        // init knight attacks
        knightAttacks[square] = maskKnightAttacks(square);

        // init king attacks
        kingAttacks[square] = maskKingAttacks(square);
    }
}

/**
 * Setting occupancies
 * @param index index of current occupency
 * @param bitsInMask bits in the current mask
 * @param attackMask bitboard of current attack mask
 * @return bitboard represents the wanted occupancy
 */
U64 setOccupancy(int index, int bitsInMask, U64 attackMask) {
    U64 occupancy = 0ULL;

    // loop over the range of bits within attack mask
    for (int count = 0; count < bitsInMask; count++) {
        // get LS1B index of attacks mask
        int square = getLSBIndex(attackMask);

        // setting off LSB in attack map
        setBitOff(attackMask, square);

        // make sure occupancy is on board
        if (index & (1 << count)) {
            // populate occupancy map
            occupancy |= (1ULL << square);
        }
    }

    // return occupancy map
    return occupancy;
}

/**
 * Init all the possible moves for slider pieces for rook or bishop
 * @param bishop flag to determine which piece to generate attack to (1 -> bishop, 0 -> rook)
 */
void initSlidersAttacks(int bishop) {
    for (int square = 0; square < 64; square++) {
        // init bishop & rook masks
        bishopMasks[square] = maskBishopAttacks(square);
        rookMasks[square] = maskRookAttacks(square);

        // init current mask
        U64 attackMask = bishop ? bishopMasks[square] : rookMasks[square];

        // init relevant occupancy bit count
        int relevantBitsCount = countBits(attackMask);

        // init occupancy indicies
        int occupancyIndicies = (1 << relevantBitsCount);

        for (int index = 0; index < occupancyIndicies; index++) {
            // bishop
            if (bishop) {
                U64 occupancy = setOccupancy(index, relevantBitsCount, attackMask);
                int magicIndex = (occupancy * bishopMagicNumbers[square]) >> (64 - bishopRelevantBits[square]);
                bishopAttacks[square][magicIndex] = bishopAttacksOnTheFly(square, occupancy);
            }

                // rook
            else {
                U64 occupancy = setOccupancy(index, relevantBitsCount, attackMask);
                int magicIndex = (occupancy * rookMagicNumbers[square]) >> (64 - rookRelevantBits[square]);
                rookAttacks[square][magicIndex] = rookAttacksOnTheFly(square, occupancy);
            }
        }
    }
}
