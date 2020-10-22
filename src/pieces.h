/**
 * Pieces constants and converting functions
 */

#ifndef NISSIMENGINECPP_PIECES_H
#define NISSIMENGINECPP_PIECES_H

#endif //NISSIMENGINECPP_PIECES_H


/**
 * enum describing all the different pieces and values
 */
enum Piece {
    /**
     * White Pawn
     */
    P,
    /**
     * White Knight
     */
    N,
    /**
     * White Bishop
     */
    B,
    /**
     * White Rook
     */
    R,
    /**
     * White Queen
     */
    Q,
    /**
     * White King
     */
    K,
    /**
     * Black Pawn
     */
    p,
    /**
     * Black knight
     */
    n,
    /**
     * Black Bishop
     */
    b,
    /**
     * Black Rook
     */
    r,
    /**
     * Black Queen
     */
    q,
    /**
     * Black King
     */
    k
};

/**
 * enum defining all different castling rights contains
 */
enum CastlingRights {
    /**
     * White King-Side Castling
     */
    WK = 1,
    /**
     * White Queen-Side Castling
     */
    WQ = 2,
    /**
     * Black King-Side Castling
     */
    BK = 4,
    /**
     * Black Queen-Side Castling
     */
    BQ = 8
};


/**
 * Array of all pieces represented by ascii characters
 */
extern char asciiPieces[12];


/**
 * array of all pieces represented by unicode characters
 */
//char *unicode_pieces[12] = {'♙', '♘', '♗', '♖', '♕', '♔', '♟', '♞', '♝', '♜', '♛', '♚'};

/**
 * converting given char to equivalent piece value
 * @param c Char represent a piece ('P' , 'q' etc)
 * @return int value represent the given piece char as int value
 */
int convertPiece(char c);

/**
 * converting given piece int value to matching char as a promotion
 * @param piece int value of given prokmoted piece
 * @return char represent the promoted piece result as lower case char
 */
char getPromotedPiece(int piece);