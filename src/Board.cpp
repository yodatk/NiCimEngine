#include "Board.h"

/**
 * 12 bitBoard: for each color 6 type of pieces
 *  0 1 2 3 4 5 6 7 8 9 10 11
 *  P,N,B,R,Q,K,p,n,b,r,q ,k
 */
U64 bitboards[12];

/**
 * 3 bitboard to tell occupencies of white pieces, black pieces, and both
 */
U64 occupancies[3];

/**
 * flag to determine current side to move (according to enum)
 */
int side;

/**
 * integer represents the current enpassant square. if none -> equal to NO_SQUARE
 */
int enpassant;

/**
 * encoded int represents the castling rights
 */
int castle;

/**
 * Hash key of the current situation on the board for searching purposes
 */
U64 hashKey;

/**
 * Table for finding 3-fold repetition and be considerate of unwanted \ desired draw
 */
U64 repetitionTable[1000]; // 1000 is a number of plies (500 moves) in the entire game

/**
 * index of the repetition table
 */
int repetitionIndex;

/**
 * var to store the current ply of the search
 */
int ply;


/**
 * Array for fast update of the castling rights after a move
 */
const int castlingRights[64] = {
        7, 15, 15, 15, 3, 15, 15, 11,
        15, 15, 15, 15, 15, 15, 15, 15,
        15, 15, 15, 15, 15, 15, 15, 15,
        15, 15, 15, 15, 15, 15, 15, 15,
        15, 15, 15, 15, 15, 15, 15, 15,
        15, 15, 15, 15, 15, 15, 15, 15,
        15, 15, 15, 15, 15, 15, 15, 15,
        13, 15, 15, 15, 12, 15, 15, 14};


/**
 * print the current situation on the board
 */
void printBoard() {
    printf("\n");
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            int square = rank * 8 + file;
            if (!file) {
                // print number of file
                printf("  %d ", 8 - rank);
            }

            int piece = -1;
            for (int bitboardPiece = P; bitboardPiece <= k; bitboardPiece++) {
                // printing piece in square if there is any
                if (getBit(bitboards[bitboardPiece], square)) {
                    // get piece code
                    piece = bitboardPiece;
                }
            }
            printf(" %c", (piece == -1) ? '.' : asciiPieces[piece]);
        }
        printf("\n");
    }

    printf("\n     A B C D E F G H\n\n");

    // printing board stats
    printf("     Side:     %s\n", !side ? "WHITE" : "BLACK");
    printf("     Enpassant:   %s\n", (enpassant != NO_SQUARE) ? squareToCoordinates[enpassant] : "no");
    printf("     Castling:  %c%c%c%c\n\n", (castle & WK) ? 'K' : '-',
           (castle & WQ) ? 'Q' : '-',
           (castle & BK) ? 'k' : '-',
           (castle & BQ) ? 'q' : '-');
    printf("     Hash key:  %llx\n\n", hashKey);
}

/**
 * reset all data of the board to an empty board
 */
void resetBoard() {
    memset(bitboards, 0ULL, sizeof(bitboards));
    memset(occupancies, 0ULL, sizeof(occupancies));
    side = 0;
    enpassant = NO_SQUARE;
    castle = 0;
    repetitionIndex = 0;
    memset(repetitionTable, 0ULL, sizeof(repetitionTable));
}


/**
 * parse a given fen string and imply it on the board
 * @param fen string to represent situation to parse on the board
 */
void parseFen(char *fen) {
    resetBoard();
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            int square = rank * 8 + file;
            if ((*fen >= 'a' && *fen <= 'z') || (*fen >= 'A' && *fen <= 'Z')) {
                // checking to parse a piece
                int piece = convertPiece(*fen);
                if (piece != -1) {
                    setBitOn(bitboards[piece], square);
                }
                fen++;
            }

            // checking for square skippers
            if (*fen >= '0' && *fen <= '9') {
                int offset = *fen - '0';
                int piece = -1;
                for (int bb_piece = P; bb_piece <= k; bb_piece++) {
                    // if there is a piece on current square, skip the fiven number
                    if (getBit(bitboards[bb_piece], square))
                        // get piece code
                        piece = bb_piece;
                }
                if (piece == -1) {
                    // if there is no piece on given square, skip 1 less
                    file--;
                }
                file += offset;
                fen++;
            }

            // check for rank seperator
            if (*fen == '/') {
                fen++;
            }
        }
    }
    fen++;
    // parsing board stats:
    // parse side
    (*fen == 'w') ? (side = WHITE) : (side = BLACK);

    fen += 2;

    // parse castling rights
    while (*fen != ' ') {
        switch (*fen) {
            case 'K':
                castle |= WK;
                break;
            case 'Q':
                castle |= WQ;
                break;
            case 'k':
                castle |= BK;
                break;
            case 'q':
                castle |= BQ;
                break;
            case '-':
                break;
        }
        fen++;
    }
    fen++;

    // parse enpassant square
    if (*fen != '-') {
        int file = fen[0] - 'a';
        int rank = 8 - (fen[1] - '0');
        enpassant = rank * 8 + file;
    } else {
        // no enpassant square
        enpassant = NO_SQUARE;
    }
    // updating occupencies according to al pieces that were loaded, and diffrintiate between white, black, and both
    for (int piece = P; piece <= K; piece++) {
        occupancies[WHITE] |= bitboards[piece];
    }

    for (int piece = p; piece <= k; piece++) {
        occupancies[BLACK] |= bitboards[piece];
    }
    occupancies[BOTH] |= occupancies[WHITE];
    occupancies[BOTH] |= occupancies[BLACK];
    // finally -> generate hashkey for current board
    hashKey = generate_hash_key();
}



/**
 * debug function to check all the attacked square by a given side
 * @param side attacking side
 */
void printAttackedSquares(int side) {
    printf("\n");
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            int square = rank * 8 + file;
            if (!file) {
                printf("  %d ", 8 - rank);
            }
            printf(" %d", isSquareAttacked(square, side) ? 1 : 0);
        }
        printf("\n");
    }
    printf("\n     A B C D E F G H\n\n");
}




/**
 * print current encoded move in a viewable way
 * @param move encoded move to print
 */
void printMove(int move) {
    if (getMovePromoted(move))
        printf("%s%s%c", squareToCoordinates[getMoveSource(move)],
               squareToCoordinates[getMoveTarget(move)],
               getPromotedPiece(getMovePromoted(move)));
    else
        printf("%s%s", squareToCoordinates[getMoveSource(move)],
               squareToCoordinates[getMoveTarget(move)]);
}

/**
 * print all moves in given moves list for debugging purposes
 * @param moveList move list to print
 */
void print_moveList(moves *moveList) {
    if (!moveList->count) {
        printf("\n     No move in the move list!\n");
        return;
    }
    printf("\n     move    piece     capture   double    enpass    castling\n\n");

    for (int move_count = 0; move_count < moveList->count; move_count++) {
        int move = moveList->moves[move_count];
        printf("      %s%s%c   %c         %d         %d         %d         %d\n",
               squareToCoordinates[getMoveSource(move)],
               squareToCoordinates[getMoveTarget(move)],
               getMovePromoted(move) ? getPromotedPiece(getMovePromoted(move)) : ' ',
               asciiPieces[getMovePiece(move)],
               getMoveCapture(move) ? 1 : 0,
               getMoveDouble(move) ? 1 : 0,
               getMoveEnpassant(move) ? 1 : 0,
               getMoveCastling(move) ? 1 : 0);
    }
    printf("\n\n     Total number of moves: %d\n\n", moveList->count);
}

/**
 * making given encoded move on board and change data structers accordingly
 * @param move integer represents an encoded move to make
 * @param moveFlag : flag to determine if to do only capture moves, or all moves
 * @return 1 if move was done, 0 otherwise
 */
int makeMove(int move, int moveFlag) {
    // if quiet moves
    if (moveFlag == ALL_MOVES) {
        // saving board state
        copyBoard();

        // extracting move data
        int sourceSquare = getMoveSource(move);
        int targetSquare = getMoveTarget(move);
        int piece = getMovePiece(move);
        int promotedPiece = getMovePromoted(move);
        int capture = getMoveCapture(move);
        int doublePush = getMoveDouble(move);
        int enpass = getMoveEnpassant(move);
        int castling = getMoveCastling(move);

        // move piece
        setBitOff(bitboards[piece], sourceSquare);
        setBitOn(bitboards[piece], targetSquare);

        // hash piece movement
        hashKey ^= pieceKeys[piece][sourceSquare];
        hashKey ^= pieceKeys[piece][targetSquare];


        if (capture) {
            // if handling capture moves -> pick up bitboard piece index ranges depending on side
            int startPiece, endPiece;
            if (side == WHITE) {
                startPiece = p;
                endPiece = k;
            } else {
                startPiece = P;
                endPiece = K;
            }


            for (int currentPiece = startPiece; currentPiece <= endPiece; currentPiece++) {
                // searching for captured piece to disable it's hash on hash key, and remove it from board
                if (getBit(bitboards[currentPiece], targetSquare)) {
                    setBitOff(bitboards[currentPiece], targetSquare);
                    hashKey ^= pieceKeys[currentPiece][targetSquare];
                    break;
                }
            }
        }


        if (promotedPiece) {
            // if handle pawn promotions
            if (side == WHITE) {
                // erase the pawn  and de-hash it
                setBitOff(bitboards[P], targetSquare);
                hashKey ^= pieceKeys[P][targetSquare];
            } else {
                // black
                setBitOff(bitboards[p], targetSquare);
                hashKey ^= pieceKeys[p][targetSquare];
            }

            // set up promoted piece on chess board and hash it
            setBitOn(bitboards[promotedPiece], targetSquare);
            hashKey ^= pieceKeys[promotedPiece][targetSquare];
        }

        if (enpass) {
            // if handle enpassant captures -> erase the pawn depending on side to move
            (side == WHITE) ? setBitOff(bitboards[p], targetSquare + 8) : setBitOff(bitboards[P], targetSquare - 8);
            if (side == WHITE) {
                setBitOff(bitboards[p], targetSquare + 8);
                hashKey ^= pieceKeys[p][targetSquare + 8];
            } else {
                setBitOff(bitboards[P], targetSquare - 8);
                hashKey ^= pieceKeys[P][targetSquare - 8];
            }
        }

        // reset enpassant square and de-hash it if necessary
        if (enpassant != NO_SQUARE) {
            hashKey ^= enpassantKeys[enpassant];
        }

        enpassant = NO_SQUARE;
        if (doublePush) {
            // if double push ->  create and hash new enpassant square
            if (side == WHITE) {
                enpassant = targetSquare + 8;
                hashKey ^= enpassantKeys[targetSquare + 8];
            } else {
                enpassant = targetSquare - 8;
                hashKey ^= enpassantKeys[targetSquare - 8];
            }
        }

        if (castling) {
            // if castling-> update rook position accordingly
            switch (targetSquare) {
                // WHITE castles king side
                case (G1):
                    // move H rook
                    setBitOff(bitboards[R], H1);
                    setBitOn(bitboards[R], F1);
                    // hash rook
                    hashKey ^= pieceKeys[R][H1];
                    hashKey ^= pieceKeys[R][F1];
                    break;
                    // WHITE castles queen side
                case (C1):
                    // move A rook
                    setBitOff(bitboards[R], A1);
                    setBitOn(bitboards[R], D1);

                    // hash rook
                    hashKey ^= pieceKeys[R][A1];
                    hashKey ^= pieceKeys[R][D1];
                    break;

                    // BLACK castles king side
                case (G8):
                    // move H rook
                    setBitOff(bitboards[r], H8);
                    setBitOn(bitboards[r], F8);

                    // hash rook
                    hashKey ^= pieceKeys[r][H8];
                    hashKey ^= pieceKeys[r][F8];
                    break;

                    // BLACK castles queen side
                case (C8):
                    // move A rook
                    setBitOff(bitboards[r], A8);
                    setBitOn(bitboards[r], D8);

                    // hash rook
                    hashKey ^= pieceKeys[r][A8]; // remove rook from a8 from hash key
                    hashKey ^= pieceKeys[r][D8]; // put rook on d8 into a hash key
                    break;
            }
        }

        // handeling hashing new castling right's and cancel the old ones  & update hashing as well
        hashKey ^= castleKeys[castle];

        castle &= castlingRights[sourceSquare];
        castle &= castlingRights[targetSquare];

        hashKey ^= castleKeys[castle];

        // reset occupancies
        memset(occupancies, 0ULL, 24);

        // handle white occupencies
        for (int currentWhitePiece = P; currentWhitePiece <= K; currentWhitePiece++) {
            occupancies[WHITE] |= bitboards[currentWhitePiece];
        }

        for (int currentBlackPiece = p; currentBlackPiece <= k; currentBlackPiece++) {
            occupancies[BLACK] |= bitboards[currentBlackPiece];
        }

        occupancies[BOTH] |= occupancies[WHITE];
        occupancies[BOTH] |= occupancies[BLACK];

        // change side and hash it
        side ^= 1;
        hashKey ^= sideKey;

        // if move is invalid -> restore back to before movement and return invalid move flag
        if (isSquareAttacked((side == WHITE) ? getLSBIndex(bitboards[k]) : getLSBIndex(bitboards[K]), side)) {
            restoreBoard();
            return 0;
        } else {
            // return valid move flag
            return 1;
        }

    }

        // if need to move only capture move -> check if current move is a capture move
    else {
        if (getMoveCapture(move)) {
            // move is indeed a capture move -> proceed as usual
            return makeMove(move, ALL_MOVES);
        } else {
            // current move is not a capture -> don't make move
            return 0;
        }

    }
}



