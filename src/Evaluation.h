/**
 * In charge of board evaluation to help in best move search
 */
#ifndef NISSIMENGINECPP_EVALUATION_H
#define NISSIMENGINECPP_EVALUATION_H

#include "Board.h"
#include "NnueEval.h"

#define NNUE_FILE "nn-eba324f53044.nnue"

/**
 * enum for game phases to flexible evaluation
 */
enum {
    OPENING,
    ENDGAME,
    MIDDLEGAME
};

/**
 * enum to symbolize white and black kings indexes in the nnue pieces array
 */
enum {
    WHITE_KING,
    BLACK_KING,
};

/**
 * enum for all pieces type
 */
enum {
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
};

/**
 * array to convert pieces value to NNUE evaluation of stockfish
 */
extern int nnuePieces[12];

/**
 * array to convert squares value to NNUE evaluation of stockfish
 */
extern int nnueSquares[64];

/**
 * 2D array to get piece value [game phase][piece]
 */
extern const int materialScore[2][12];


/**
 * Constants to determine if the current board is in opening, middle game or end game
 */
extern const int openingPhaseScore;
extern const int endgamePhaseScore;


/**
 * Table to calculate score according to piece position [game phase][piece][square]
 */
extern const int positionalScore[2][6][64];

/**
 * mirror positional score tables for opposite side, for black side easy evaluation
 */
extern const int mirrorScore[128];


/**
 * File masks for each square
 */
extern U64 fileMasks[64];

/**
 * Rank masks for each square
 */
extern U64 rankMasks[64];

/**
 * Masks for isolated pawn evaluation
 */
extern U64 isolatedMasks[64];

/**
 * Masks for White Passed Pawns
 */
extern U64 whitePassedMasks[64];

/**
 * Masks for Black Passed Pawns
 */
extern U64 blackPassedMasks[64];

/**
 * get rank according to square array
 */
extern const int getRank[64];


/**
 * Double pawn panelty according to game phase
 */
static const int DOUBLE_PAWN_PENALTY_OPENING = -5;
static const int DOUBLE_PAWN_PENALTY_ENDGAME = -10;

/**
 * Isolated pawn panelty according to game phase
 */
static const int ISOLATED_PAWN_PENALTY_OPENING = -5;
static const int ISOLATED_PAWN_PENALTY_ENDGAME = -10;

/**
 * Passed pawn bonus array
 */
static const int passedPawnBonus[8] = {0, 10, 30, 50, 75, 100, 150, 200};

/**
 * Semi open file score (for rook bonus or king penalty)
 */
static const int SEMI_OPEN_FILE_SCORE = 10;

/**
 * Open file score (for rook bonus or king penalty)
 */
static const int OPEN_FILE_SCORE = 15;

/**
 * Bishop and queen unit for mobility eval
 */
static const int BISHOP_UNIT = 4;
static const int QUEEN_UNIT = 9;

static const int BISHOP_MOBILITY_OPENING = 5;
static const int BISHOP_MOBILITY_ENDGAME = 5;
static const int QUEEN_MOBILITY_OPENING = 1;
static const int QUEEN_MOBILITY_ENDGAME = 2;

/**
 * Bonus for king safety
 */
static const int KING_SHIELD_BONUS = 5;


/**
 * Setting Mask for given File or given Rank
 * @param fileNumber int represent the file to create mask for. if none, is -1
 * @param rankNumber int represent the rank to create mask for. if none, is -1
 * @return Unsigned long long which is the output mask for the given rank \ file
 */
extern U64 setFileRankMask(int fileNumber, int rankNumber);

/**
 * init all masks for the evaluation process
 */
extern void initEvaluationMasks();


/**
 * Calculate the current value of Officers(knight to queen both sides) to determine the current game phase (opening, endgame, middlegame)
 * @return integer represents the value of the current game phase
 */
static inline int getGamePhaseScore() {

    int whitePieceScores = 0, blackPieceScores = 0;

    // counting white officers
    for (int piece = N; piece <= Q; piece++) {
        whitePieceScores += countBits(bitboardsPieces[piece]) * materialScore[OPENING][piece];
    }

    // counting black officsers
    for (int piece = n; piece <= q; piece++) {
        blackPieceScores += countBits(bitboardsPieces[piece]) * -materialScore[OPENING][piece];
    }

    return whitePieceScores + blackPieceScores;
}

/**
 * Evaluate current position on board according to pieces value and pieces position
 * @return score of the current board
 */
static inline int evaluate() {
    U64 bitboard;

    int piece, square;

    // arrays and fields for stockfish nnue evaluation
    int pieces[33];
    int squares[33];
    int index = 2;

    for (int pieceType = P; pieceType <= k; pieceType++) {
        // init piece bitboard copy
        bitboard = bitboardsPieces[pieceType];
        while (bitboard) {
            // getting piece and location
            piece = pieceType;
            square = getLSBIndex(bitboard);

            if (piece == K) {
                // handeling white king nnue
                pieces[WHITE_KING] = nnuePieces[piece];
                squares[WHITE_KING] = nnueSquares[square];
            } else if (piece == k) {
                // handeling black king nnue
                pieces[BLACK_KING] = nnuePieces[piece];
                squares[BLACK_KING] = nnueSquares[square];
            } else {
                // converting rest of the pieces to converted squares for nnue evaluation
                pieces[index] = nnuePieces[piece];
                squares[index] = nnueSquares[square];
                index++;
            }
            // removing piece
            setBitOff(bitboard, square);
        }
    }

    // set final zero on both nnue array
    pieces[index] = 0;
    squares[index] = 0;

    // evaluating with NNUE, with fifty move rule considiration
    return (evaluateNNUE(side, pieces, squares) * (100-fiftyRuleCounter)/100);
}

static inline int evaluateOnLowTime() {
    // get game phase score
    int gamePhaseScore = getGamePhaseScore();

    // determine game phase
    int gamePhase = -1;

    if (gamePhaseScore > openingPhaseScore) {
        gamePhase = OPENING;
    } else if (gamePhaseScore < endgamePhaseScore) {
        gamePhase = ENDGAME;
    } else {
        gamePhase = MIDDLEGAME;
    }


    int score, scoreOpening = 0, scoreEndgame = 0;

    U64 bitboard;

    int piece, square;

    int doublePawns;

    for (int pieceType = P; pieceType <= k; pieceType++) {
        // init piece bitboard copy
        bitboard = bitboardsPieces[pieceType];


        while (bitboard) {
            // getting piece and location
            piece = pieceType;
            square = getLSBIndex(bitboard);

            // get opening/endgame material score
            scoreOpening += materialScore[OPENING][piece];
            scoreEndgame += materialScore[ENDGAME][piece];

            // score positional piece scores, and special cases pieces
            switch (piece) {
                // evaluate WHITE pawns
                case P:
                    // positional scoring
                    scoreOpening += positionalScore[OPENING][PAWN][square];
                    scoreEndgame += positionalScore[ENDGAME][PAWN][square];

                    // double pawn penalty
                    doublePawns = countBits(bitboardsPieces[P] & fileMasks[square]);
                    if (doublePawns > 1) {
                        scoreOpening += (doublePawns - 1) * DOUBLE_PAWN_PENALTY_OPENING;
                        scoreEndgame += (doublePawns - 1) * DOUBLE_PAWN_PENALTY_ENDGAME;
                    }

                    // isolated pawn penalty
                    if ((bitboardsPieces[P] & isolatedMasks[square]) == 0) {
                        scoreOpening += ISOLATED_PAWN_PENALTY_OPENING;
                        scoreEndgame += ISOLATED_PAWN_PENALTY_ENDGAME;
                    }
                    // passed pawn bonus
                    if ((whitePassedMasks[square] & bitboardsPieces[p]) == 0) {
                        scoreOpening += passedPawnBonus[getRank[square]];
                        scoreEndgame += passedPawnBonus[getRank[square]];
                    }

                    break;


                case N:
                    // positional scoring
                    scoreOpening += positionalScore[OPENING][KNIGHT][square];
                    scoreEndgame += positionalScore[ENDGAME][KNIGHT][square];

                    break;


                case B:
                    // positional scoring
                    scoreOpening += positionalScore[OPENING][BISHOP][square];
                    scoreEndgame += positionalScore[ENDGAME][BISHOP][square];

                    // mobility bonus
                    scoreOpening += (countBits(getBishopAttacks(square, occupancies[BOTH])) - BISHOP_UNIT) *
                                    BISHOP_MOBILITY_OPENING;
                    scoreEndgame += (countBits(getBishopAttacks(square, occupancies[BOTH])) - BISHOP_UNIT) *
                                    BISHOP_MOBILITY_ENDGAME;

                    break;


                case R:
                    // positional scoring
                    scoreOpening += positionalScore[OPENING][ROOK][square];
                    scoreEndgame += positionalScore[ENDGAME][ROOK][square];

                    // semi and full open files bonus
                    if ((bitboardsPieces[P] & fileMasks[square]) == 0) {
                        scoreOpening += SEMI_OPEN_FILE_SCORE;
                        scoreEndgame += SEMI_OPEN_FILE_SCORE;
                    }
                    if (((bitboardsPieces[P] | bitboardsPieces[p]) & fileMasks[square]) == 0) {
                        scoreOpening += OPEN_FILE_SCORE;
                        scoreEndgame += OPEN_FILE_SCORE;
                    }

                    break;

                case Q:
                    // positional scoring
                    scoreOpening += positionalScore[OPENING][QUEEN][square];
                    scoreEndgame += positionalScore[ENDGAME][QUEEN][square];

                    // mobility bonus
                    scoreOpening += (countBits(getQueenAttacks(square, occupancies[BOTH])) - QUEEN_UNIT) *
                                    QUEEN_MOBILITY_OPENING;
                    scoreEndgame += (countBits(getQueenAttacks(square, occupancies[BOTH])) - QUEEN_UNIT) *
                                    QUEEN_MOBILITY_ENDGAME;

                    break;

                case K:
                    // positional scoring
                    scoreOpening += positionalScore[OPENING][KING][square];
                    scoreEndgame += positionalScore[ENDGAME][KING][square];

                    // semi and open file penalty
                    if ((bitboardsPieces[P] & fileMasks[square]) == 0) {

                        scoreOpening -= SEMI_OPEN_FILE_SCORE;
                        scoreEndgame -= SEMI_OPEN_FILE_SCORE;
                    }
                    if (((bitboardsPieces[P] | bitboardsPieces[p]) & fileMasks[square]) == 0) {
                        scoreOpening -= OPEN_FILE_SCORE;
                        scoreEndgame -= OPEN_FILE_SCORE;
                    }

                    // king safety bonus
                    scoreOpening += countBits(kingAttacks[square] & occupancies[WHITE]) * KING_SHIELD_BONUS;
                    scoreEndgame += countBits(kingAttacks[square] & occupancies[WHITE]) * KING_SHIELD_BONUS;

                    break;

                case p:
                    // positional scoring
                    scoreOpening -= positionalScore[OPENING][PAWN][mirrorScore[square]];
                    scoreEndgame -= positionalScore[ENDGAME][PAWN][mirrorScore[square]];

                    // double pawn penalty
                    doublePawns = countBits(bitboardsPieces[p] & fileMasks[square]);

                    if (doublePawns > 1) {
                        scoreOpening -= (doublePawns - 1) * DOUBLE_PAWN_PENALTY_OPENING;
                        scoreEndgame -= (doublePawns - 1) * DOUBLE_PAWN_PENALTY_ENDGAME;
                    }

                    // isolated pawn penalty
                    if ((bitboardsPieces[p] & isolatedMasks[square]) == 0) {
                        // give an isolated pawn penalty
                        scoreOpening -= ISOLATED_PAWN_PENALTY_OPENING;
                        scoreEndgame -= ISOLATED_PAWN_PENALTY_ENDGAME;
                    }

                    // passed pawn bonus
                    if ((blackPassedMasks[square] & bitboardsPieces[P]) == 0) {

                        scoreOpening -= passedPawnBonus[getRank[square]];
                        scoreEndgame -= passedPawnBonus[getRank[square]];
                    }

                    break;


                case n:
                    // give passed pawn bonus
                    scoreOpening -= positionalScore[OPENING][KNIGHT][mirrorScore[square]];
                    scoreEndgame -= positionalScore[ENDGAME][KNIGHT][mirrorScore[square]];

                    break;


                case b:
                    // give passed pawn bonus
                    scoreOpening -= positionalScore[OPENING][BISHOP][mirrorScore[square]];
                    scoreEndgame -= positionalScore[ENDGAME][BISHOP][mirrorScore[square]];

                    // mobility bonus
                    scoreOpening -= (countBits(getBishopAttacks(square, occupancies[BOTH])) - BISHOP_UNIT) *
                                    BISHOP_MOBILITY_OPENING;
                    scoreEndgame -= (countBits(getBishopAttacks(square, occupancies[BOTH])) - BISHOP_UNIT) *
                                    BISHOP_MOBILITY_ENDGAME;

                    break;

                case r:
                    // positional scoring
                    scoreOpening -= positionalScore[OPENING][ROOK][mirrorScore[square]];
                    scoreEndgame -= positionalScore[ENDGAME][ROOK][mirrorScore[square]];

                    // semi open file and open file bonus
                    if ((bitboardsPieces[p] & fileMasks[square]) == 0) {
                        scoreOpening -= SEMI_OPEN_FILE_SCORE;
                        scoreEndgame -= SEMI_OPEN_FILE_SCORE;
                    }
                    if (((bitboardsPieces[P] | bitboardsPieces[p]) & fileMasks[square]) == 0) {
                        scoreOpening -= OPEN_FILE_SCORE;
                        scoreEndgame -= OPEN_FILE_SCORE;
                    }

                    break;
                case q:
                    // positional scoring
                    scoreOpening -= positionalScore[OPENING][QUEEN][mirrorScore[square]];
                    scoreEndgame -= positionalScore[ENDGAME][QUEEN][mirrorScore[square]];

                    // mobility bonus
                    scoreOpening -= (countBits(getQueenAttacks(square, occupancies[BOTH])) - QUEEN_UNIT) *
                                    QUEEN_MOBILITY_OPENING;
                    scoreEndgame -= (countBits(getQueenAttacks(square, occupancies[BOTH])) - QUEEN_UNIT) *
                                    QUEEN_MOBILITY_ENDGAME;

                    break;

                case k:
                    // positional scoring
                    scoreOpening -= positionalScore[OPENING][KING][mirrorScore[square]];
                    scoreEndgame -= positionalScore[ENDGAME][KING][mirrorScore[square]];

                    // semi open file and open file penalty
                    if ((bitboardsPieces[p] & fileMasks[square]) == 0) {
                        // add semi open file penalty
                        scoreOpening += SEMI_OPEN_FILE_SCORE;
                        scoreEndgame += SEMI_OPEN_FILE_SCORE;
                    }
                    if (((bitboardsPieces[P] | bitboardsPieces[p]) & fileMasks[square]) == 0) {
                        scoreOpening += OPEN_FILE_SCORE;
                        scoreEndgame += OPEN_FILE_SCORE;
                    }

                    // king safety bonus
                    scoreOpening -= countBits(kingAttacks[square] & occupancies[BLACK]) * KING_SHIELD_BONUS;
                    scoreEndgame -= countBits(kingAttacks[square] & occupancies[BLACK]) * KING_SHIELD_BONUS;

                    break;
            }

            // removing piece
            setBitOff(bitboard, square);
        }
    }


    // interpolate score in the middle-game
    if (gamePhase == MIDDLEGAME) {
        score = (scoreOpening * gamePhaseScore +
                 scoreEndgame * (openingPhaseScore - gamePhaseScore)) /
                openingPhaseScore;
    } else if (gamePhase == OPENING) {
        score = scoreOpening;
    } else if (gamePhase == ENDGAME) {
        score = scoreEndgame;
    }


    // return final evaluation based on side
    return (side == WHITE) ? score : -score;
}

#endif //NISSIMENGINECPP_EVALUATION_H
