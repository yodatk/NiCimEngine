#include "Pieces.h"

/**
 * Array of all pieces represented by ascii characters
 */
char asciiPieces[12] = {'P', 'N', 'B', 'R', 'Q', 'K', 'p', 'n', 'b', 'r', 'q', 'k'};


/**
 * converting given char to equivalent piece value
 * @param c Char represent a piece ('P' , 'q' etc)
 * @return int value represent the given piece char as int value
 */
int convertPiece(char c) {
    switch (c) {
        case 'P':
            return P;
        case 'N':
            return N;
        case 'B':
            return B;
        case 'R':
            return R;
        case 'Q':
            return Q;
        case 'K':
            return K;
        case 'p':
            return p;
        case 'n':
            return n;
        case 'b':
            return b;
        case 'r':
            return r;
        case 'q':
            return q;
        case 'k':
            return k;
        default:
            return -1;
    }
}

/**
 * converting given piece int value to matching char as a promotion
 * @param piece int value of given prokmoted piece
 * @return char represent the promoted piece result as lower case char
 */
char getPromotedPiece(int piece) {
    switch (piece) {
        case Q:
        case q:
            return 'q';
        case R:
        case r:
            return 'r';
        case B:
        case b:
            return 'b';
        case N:
        case n:
            return 'n';
        default:
            return ' ';

    }
}