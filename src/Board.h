/**
 * in charge of board state -> all bitboard and variables and data to describe the board state and data manipulation
 */
#ifndef NISSIMENGINECPP_BOARD_H
#define NISSIMENGINECPP_BOARD_H


#include <cstdlib>
#include <cstring>
#include <cstdio>
#include "Pieces.h"
#include "Bitboard.h"
#include "Squares.h"
#include "EnumsAndConstants.h"
#include "ZorbiestKeys.h"
#include "Attacks.h"

/**
 * macro to copy Board state
 */
#define copyBoard()                                                    \
    U64 bitboardsCopy[12], occupanciesCopy[3];                        \
    int sideCopy, enpassantCopy, castleCopy,fiftyRuleCounterCopy;     \
    memcpy(bitboardsCopy, bitboards, 96);                              \
    memcpy(occupanciesCopy, occupancies, 24);                          \
    sideCopy = side, enpassantCopy = enpassant, castleCopy = castle;   \
    fiftyRuleCounterCopy = fiftyRuleCounter;                            \
    U64 hashKeyCopy = hashKey;


/**
 * macro to restore board state from copy
 */
#define restoreBoard()                                                     \
    memcpy(bitboards, bitboardsCopy, 96);                              \
    memcpy(occupancies, occupanciesCopy, 24);                              \
    side = sideCopy, enpassant = enpassantCopy, castle = castleCopy;       \
    fiftyRuleCounter= fiftyRuleCounterCopy;                               \
    hashKey = hashKeyCopy;



/**
 * macro to convert given parameters to a single integer that represent the wanted move
 */
#define encodeMove(source, target, piece, promoted, capture, doublePush, enpassant, castling) \
    (source) |                                                                             \
        (target << 6) |                                                                    \
        (piece << 12) |                                                                    \
        (promoted << 16) |                                                                 \
        (capture << 20) |                                                                  \
        (doublePush << 21) |                                                               \
        (enpassant << 22) |                                                                \
        (castling << 23)

/**
 * extracting move source from encoded move
 */
#define getMoveSource(move) (move & 0x3f)

/**
 * extracting move target from encoded move
 */
#define getMoveTarget(move) ((move & 0xfc0) >> 6)

/**
 * extracting move piece from encoded move
 */
#define getMovePiece(move) ((move & 0xf000) >> 12)

/**
 * extracting move promoted piece from encoded move
 */
#define getMovePromoted(move) ((move & 0xf0000) >> 16)

/**
 * extracting isCapture flag  from encoded move
 */
#define getMoveCapture(move) (move & 0x100000)

/**
 * extracting isDoublePush flag from encoded move
 */
#define getMoveDouble(move) (move & 0x200000)

/**
 * extracting EnPassant flag from encoded move
 */
#define getMoveEnpassant(move) (move & 0x400000)

/**
 * extracting castling flag from encoded move
 */
#define getMoveCastling(move) (move & 0x800000)


/**
 * Enum to determine move type for "makeMove" function
 */
enum {
    ALL_MOVES,
    CAPTURES_ONLY
};


/**
 * data structure for storing moves available on current board
 */
typedef struct {
    int moves[256];
    int count;
} moves;


/**
 * 12 bitBoard: for each color 6 type of pieces
 *  0 1 2 3 4 5 6 7 8 9 10 11
 *  P,N,B,R,Q,K,p,n,b,r,q ,k
 */
extern U64 bitboards[12];

/**
 * 3 bitboard to tell occupencies of white pieces, black pieces, and both
 */
extern U64 occupancies[3];

/**
 * flag to determine current side to move (according to enum)
 */
extern int side;

/**
 * integer represents the current enpassant square. if none -> equal to NO_SQUARE
 */
extern int enpassant;

/**
 * encoded int represents the castling rights
 */
extern int castle;

/**
 * Hash key of the current situation on the board for searching purposes
 */
extern U64 hashKey;

/**
 * Table for finding 3-fold repetition and be considerate of unwanted \ desired draw
 */
extern U64 repetitionTable[1000]; // 1000 is a number of plies (500 moves) in the entire game

/**
 * index of the repetition table
 */
extern int repetitionIndex;


/**
 * Array for fast update of the castling rights after a move
 */
extern const int castlingRights[64];

/**
 * var to store the current ply of the search
 */
extern int ply;

/**
 * counter to avoid drawing \ achiving a draw because of the fifty move rule
 */
extern int fiftyRuleCounter;

/**
 * print the current situation on the board
 */
void printBoard();


/**
 * reset all data of the board to an empty board
 */
void resetBoard();


/**
 * parse a given fen string and imply it on the board
 * @param fen string to represent situation to parse on the board
 */
void parseFen(char *fen);


/**
 * check if a given square is attacked by a given side pieces
 * @param square square to check if attacked
 * @param side attacking side
 * @return 0 if square not attacked, 1 other wise
 */
static inline int isSquareAttacked(int square, int side) {

    if ((side == WHITE) && (pawnAttacks[BLACK][square] & bitboards[P])) {
        // if attacked by white pawns as black
        return 1;
    }
    if ((side == BLACK) && (pawnAttacks[WHITE][square] & bitboards[p])) {
        // if attacked by white pawns as black
        return 1;
    }
    if (knightAttacks[square] & ((side == WHITE) ? bitboards[N] : bitboards[n])) {
        // if attacked by knights
        return 1;
    }
    if (getBishopAttacks(square, occupancies[BOTH]) & ((side == WHITE) ? bitboards[B] : bitboards[b])) {
        // if attacked by bishops
        return 1;
    }
    if (getRookAttacks(square, occupancies[BOTH]) & ((side == WHITE) ? bitboards[R] : bitboards[r])) {
        // if attacked by rooks
        return 1;
    }
    if (getQueenAttacks(square, occupancies[BOTH]) & ((side == WHITE) ? bitboards[Q] : bitboards[q])) {
        // if attacked by bishops
        return 1;
    }


    if (kingAttacks[square] & ((side == WHITE) ? bitboards[K] : bitboards[k])) {
        // attacked by kings
        return 1;
    }

    // if none of the above -> not attacked by given side
    return 0;
}

/**
 * debug function to check all the attacked square by a given side
 * @param side attacking side
 */
void printAttackedSquares(int side);

/**
 * add encoded move to given moves list
 */
static inline void addMove(moves * moveList, int
move) {
moveList->moves[moveList->count] =
move;
moveList->count++;
}

/**
 * print current encoded move in a viewable way
 * @param move encoded move to print
 */
void printMove(int move);

/**
 * print all moves in given moves list for debugging purposes
 * @param moveList move list to print
 */
void printMoveList(moves * moveList);

/**
 * making given encoded move on board and change data structers accordingly
 * @param move integer represents an encoded move to make
 * @param moveFlag : flag to determine if to do only capture moves, or all moves
 * @return 1 if move was done, 0 otherwise
 */
int makeMove(int move, int moveFlag);

/**
 * Generating all moves possible in the current board situation, and put result in the given movelist
 * @param moveList movelist to put generated moves in
 */
static inline void generateMoves(moves * moveList) {
    moveList->count = 0;

    int sourceSquare, targetSquare;
    U64 bitboard, attacks;

    for (int piece = P; piece <= k; piece++) {
        // goe over each piece's bitboard
        bitboard = bitboards[piece];


        if (side == WHITE) {
            // generating white pawn move
            if (piece == P) {
                while (bitboard) {
                    // as long as there are pawns on bitboard copy -> generate all moves possible with that pawn
                    sourceSquare = getLSBIndex(bitboard);

                    targetSquare = sourceSquare - 8;

                    if (!(targetSquare < A8) && !getBit(occupancies[BOTH], targetSquare)) {
                        // if pawn can move foward
                        if (sourceSquare >= A7 && sourceSquare <= H7) {
                            //if promotion is possible
                            addMove(moveList, encodeMove(sourceSquare, targetSquare, piece, Q, 0, 0, 0, 0));
                            addMove(moveList, encodeMove(sourceSquare, targetSquare, piece, R, 0, 0, 0, 0));
                            addMove(moveList, encodeMove(sourceSquare, targetSquare, piece, B, 0, 0, 0, 0));
                            addMove(moveList, encodeMove(sourceSquare, targetSquare, piece, N, 0, 0, 0, 0));
                        } else {
                            // if not a promotion move, normal move
                            addMove(moveList, encodeMove(sourceSquare, targetSquare, piece, 0, 0, 0, 0, 0));


                            if ((sourceSquare >= A2 && sourceSquare <= H2) &&
                                !getBit(occupancies[BOTH], targetSquare - 8)) {
                                // double push pawn
                                addMove(moveList,
                                        encodeMove(sourceSquare, (targetSquare - 8), piece, 0, 0, 1, 0, 0));
                            }

                        }
                    }
                    // -- checking pawn attacks
                    attacks = pawnAttacks[side][sourceSquare] & occupancies[BLACK];

                    while (attacks) {

                        targetSquare = getLSBIndex(attacks);


                        if (sourceSquare >= A7 && sourceSquare <= H7) {
                            // checking if promotion capture
                            addMove(moveList, encodeMove(sourceSquare, targetSquare, piece, Q, 1, 0, 0, 0));
                            addMove(moveList, encodeMove(sourceSquare, targetSquare, piece, R, 1, 0, 0, 0));
                            addMove(moveList, encodeMove(sourceSquare, targetSquare, piece, B, 1, 0, 0, 0));
                            addMove(moveList, encodeMove(sourceSquare, targetSquare, piece, N, 1, 0, 0, 0));
                        } else {
                            // add normal capture
                            addMove(moveList, encodeMove(sourceSquare, targetSquare, piece, 0, 1, 0, 0, 0));
                        }


                        // remove current capture possibility
                        setBitOff(attacks, targetSquare);
                    }

                    // checking for enpassant move
                    if (enpassant != NO_SQUARE) {
                        // lookup pawn attacks and bitwise AND with enpassant square (bit)
                        U64 enpassantAttacks = pawnAttacks[side][sourceSquare] & (1ULL << enpassant);

                        // make sure enpassant capture available
                        if (enpassantAttacks) {
                            // init enpassant capture target square
                            int targetEnpassant = getLSBIndex(enpassantAttacks);
                            addMove(moveList, encodeMove(sourceSquare, targetEnpassant, piece, 0, 1, 0, 1, 0));
                        }
                    }

                    // removing piece from bitboard to continue to the next pawn
                    setBitOff(bitboard, sourceSquare);
                }
            }

            if (piece == K) {
                // checking for king castling moves -> add a move only if it's a valid by csstling rights, and safe
                if (castle & WK) {
                    if (!getBit(occupancies[BOTH], F1) && !getBit(occupancies[BOTH], G1)) {
                        if (!isSquareAttacked(E1, BLACK) && !isSquareAttacked(F1, BLACK))
                            addMove(moveList, encodeMove(E1, G1, piece, 0, 0, 0, 0, 1));
                    }
                }

                if (castle & WQ) {
                    if (!getBit(occupancies[BOTH], D1) && !getBit(occupancies[BOTH], C1) &&
                        !getBit(occupancies[BOTH], B1)) {
                        if (!isSquareAttacked(E1, BLACK) && !isSquareAttacked(D1, BLACK))
                            addMove(moveList, encodeMove(E1, C1, piece, 0, 0, 0, 0, 1));
                    }
                }
            }
        } else {

            if (piece == p) {
                // checking all moves possible with black pawns
                while (bitboard) {
                    sourceSquare = getLSBIndex(bitboard);
                    targetSquare = sourceSquare + 8;

                    if (!(targetSquare > H1) && !getBit(occupancies[BOTH], targetSquare)) {
                        // pawn promotion
                        if (sourceSquare >= A2 && sourceSquare <= H2) {
                            addMove(moveList, encodeMove(sourceSquare, targetSquare, piece, q, 0, 0, 0, 0));
                            addMove(moveList, encodeMove(sourceSquare, targetSquare, piece, r, 0, 0, 0, 0));
                            addMove(moveList, encodeMove(sourceSquare, targetSquare, piece, b, 0, 0, 0, 0));
                            addMove(moveList, encodeMove(sourceSquare, targetSquare, piece, n, 0, 0, 0, 0));
                        } else {
                            // add normal move
                            addMove(moveList, encodeMove(sourceSquare, targetSquare, piece, 0, 0, 0, 0, 0));

                            // double push
                            if ((sourceSquare >= A7 && sourceSquare <= H7) &&
                                !getBit(occupancies[BOTH], targetSquare + 8)) {
                                addMove(moveList,
                                        encodeMove(sourceSquare, (targetSquare + 8), piece, 0, 0, 1, 0, 0));
                            }

                        }
                    }

                    attacks = pawnAttacks[side][sourceSquare] & occupancies[WHITE];

                    // checking pawn captures
                    while (attacks) {

                        targetSquare = getLSBIndex(attacks);

                        if (sourceSquare >= A2 && sourceSquare <= H2) {
                            // check and add capture-Promotion
                            addMove(moveList, encodeMove(sourceSquare, targetSquare, piece, q, 1, 0, 0, 0));
                            addMove(moveList, encodeMove(sourceSquare, targetSquare, piece, r, 1, 0, 0, 0));
                            addMove(moveList, encodeMove(sourceSquare, targetSquare, piece, b, 1, 0, 0, 0));
                            addMove(moveList, encodeMove(sourceSquare, targetSquare, piece, n, 1, 0, 0, 0));
                        } else {
                            // normal capture
                            addMove(moveList, encodeMove(sourceSquare, targetSquare, piece, 0, 1, 0, 0, 0));
                        }


                        //remove current attack
                        setBitOff(attacks, targetSquare);
                    }

                    // check for enpassant attack
                    if (enpassant != NO_SQUARE) {
                        U64 enpassantAttacks = pawnAttacks[side][sourceSquare] & (1ULL << enpassant);


                        if (enpassantAttacks) {
                            int targetEnpassant = getLSBIndex(enpassantAttacks);
                            addMove(moveList, encodeMove(sourceSquare, targetEnpassant, piece, 0, 1, 0, 1, 0));
                        }
                    }

                    // remove pawn from board
                    setBitOff(bitboard, sourceSquare);
                }
            }

            // castling moves for black king according to castling rights, and king safety
            if (piece == k) {
                if (castle & BK) {
                    if (!getBit(occupancies[BOTH], F8) && !getBit(occupancies[BOTH], G8)) {
                        if (!isSquareAttacked(E8, WHITE) && !isSquareAttacked(F8, WHITE)) {
                            addMove(moveList, encodeMove(E8, G8, piece, 0, 0, 0, 0, 1));
                        }

                    }
                }

                if (castle & BQ) {
                    if (!getBit(occupancies[BOTH], D8) && !getBit(occupancies[BOTH], C8) &&
                        !getBit(occupancies[BOTH], B8)) {
                        if (!isSquareAttacked(E8, WHITE) && !isSquareAttacked(D8, WHITE)) {
                            addMove(moveList, encodeMove(E8, C8, piece, 0, 0, 0, 0, 1));
                        }
                    }
                }
            }
        }

        // generate knight moves
        if ((side == WHITE) ? piece == N : piece == n) {
            while (bitboard) {
                sourceSquare = getLSBIndex(bitboard);
                attacks = knightAttacks[sourceSquare] & ((side == WHITE) ? ~occupancies[WHITE] : ~occupancies[BLACK]);
                // for each knight -> add available attacks
                while (attacks) {
                    targetSquare = getLSBIndex(attacks);
                    if (!getBit(((side == WHITE) ? occupancies[BLACK] : occupancies[WHITE]), targetSquare)) {
                        //quiet move
                        addMove(moveList, encodeMove(sourceSquare, targetSquare, piece, 0, 0, 0, 0, 0));
                    } else {
                        // capture move
                        addMove(moveList, encodeMove(sourceSquare, targetSquare, piece, 0, 1, 0, 0, 0));
                    }

                    // removing attack
                    setBitOff(attacks, targetSquare);
                }

                // removing current knight
                setBitOff(bitboard, sourceSquare);
            }
        }

        // generate bishop moves
        if ((side == WHITE) ? piece == B : piece == b) {

            while (bitboard) {
                sourceSquare = getLSBIndex(bitboard);
                attacks = getBishopAttacks(sourceSquare, occupancies[BOTH]) &
                          ((side == WHITE) ? ~occupancies[WHITE] : ~occupancies[BLACK]);

                // for each bishop -> go over available attacks
                while (attacks) {
                    // init target square
                    targetSquare = getLSBIndex(attacks);

                    // quiet move
                    if (!getBit(((side == WHITE) ? occupancies[BLACK] : occupancies[WHITE]), targetSquare)) {
                        addMove(moveList, encodeMove(sourceSquare, targetSquare, piece, 0, 0, 0, 0, 0));
                    } else {
                        // capture move
                        addMove(moveList, encodeMove(sourceSquare, targetSquare, piece, 0, 1, 0, 0, 0));
                    }


                    // removing attack
                    setBitOff(attacks, targetSquare);
                }

                // removing bishop
                setBitOff(bitboard, sourceSquare);
            }
        }

        // generate rook moves
        if ((side == WHITE) ? piece == R : piece == r) {

            while (bitboard) {
                sourceSquare = getLSBIndex(bitboard);

                attacks = getRookAttacks(sourceSquare, occupancies[BOTH]) &
                          ((side == WHITE) ? ~occupancies[WHITE] : ~occupancies[BLACK]);

                // for each rook -> go over available attacks
                while (attacks) {
                    targetSquare = getLSBIndex(attacks);

                    // quiet move
                    if (!getBit(((side == WHITE) ? occupancies[BLACK] : occupancies[WHITE]), targetSquare)) {
                        addMove(moveList, encodeMove(sourceSquare, targetSquare, piece, 0, 0, 0, 0, 0));
                    } else {
                        // capture move
                        addMove(moveList, encodeMove(sourceSquare, targetSquare, piece, 0, 1, 0, 0, 0));
                    }
                    // remove current attack
                    setBitOff(attacks, targetSquare);
                }

                // remove current rook
                setBitOff(bitboard, sourceSquare);
            }
        }

        // generate queen moves
        if ((side == WHITE) ? piece == Q : piece == q) {
            while (bitboard) {
                sourceSquare = getLSBIndex(bitboard);

                attacks = getQueenAttacks(sourceSquare, occupancies[BOTH]) &
                          ((side == WHITE) ? ~occupancies[WHITE] : ~occupancies[BLACK]);

                // for each queen -> go over all avilable attacks
                while (attacks) {
                    targetSquare = getLSBIndex(attacks);

                    // quiet move
                    if (!getBit(((side == WHITE) ? occupancies[BLACK] : occupancies[WHITE]), targetSquare)) {
                        addMove(moveList, encodeMove(sourceSquare, targetSquare, piece, 0, 0, 0, 0, 0));
                    } else {
                        // capture move
                        addMove(moveList, encodeMove(sourceSquare, targetSquare, piece, 0, 1, 0, 0, 0));
                    }
                    // remove attack
                    setBitOff(attacks, targetSquare);
                }

                // remove queen
                setBitOff(bitboard, sourceSquare);
            }
        }

        // generate king moves
        if ((side == WHITE) ? piece == K : piece == k) {
            while (bitboard) {
                sourceSquare = getLSBIndex(bitboard);

                attacks = kingAttacks[sourceSquare] & ((side == WHITE) ? ~occupancies[WHITE] : ~occupancies[BLACK]);
                // for each king -> go over all available attacks
                while (attacks) {
                    targetSquare = getLSBIndex(attacks);
                    // quiet move
                    if (!getBit(((side == WHITE) ? occupancies[BLACK] : occupancies[WHITE]), targetSquare)) {
                        addMove(moveList, encodeMove(sourceSquare, targetSquare, piece, 0, 0, 0, 0, 0));
                    } else {
                        // capture move
                        addMove(moveList, encodeMove(sourceSquare, targetSquare, piece, 0, 1, 0, 0, 0));
                    }

                    // remove current attack
                    setBitOff(attacks, targetSquare);
                }

                // remove king
                setBitOff(bitboard, sourceSquare);
            }
        }
    }
}


/**
 * Checking in repetition table if there is a risk / oppurtunity fot 3-fold repetition
 * @return '1' if there is a repetition, return '0' if not
 */
static inline int isRepetition() {
    for (int index = 0; index < repetitionIndex; index++) {
        if (repetitionTable[index] == hashKey) {
            // if we found the hash key same with a current -> found a repetition
            return 1;
        }
    }
    return 0;
}

#endif //NISSIMENGINECPP_BOARD_H
