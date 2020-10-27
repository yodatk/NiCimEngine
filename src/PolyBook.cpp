

#include "PolyBook.h"

/**
 * converting pieces to poly pieces for opening book
 */
const int PolyTypeOfPiece[12] = {
        1, 3, 5, 7, 9, 11, 0, 2, 4, 6, 8, 10
};

/**
 * number of entries in the book
 */
long NUM_OF_ENTRIES;

/**
 * pointer to the current entry
 */
BookEntry* entries;

/**
 * array to convert squares value to NNUE evaluation of stockfish
 */
int reversedSquares[64] = {
        A1, B1, C1, D1, E1, F1, G1, H1,
        A2, B2, C2, D2, E2, F2, G2, H2,
        A3, B3, C3, D3, E3, F3, G3, H3,
        A4, B4, C4, D4, E4, F4, G4, H4,
        A5, B5, C5, D5, E5, F5, G5, H5,
        A6, B6, C6, D6, E6, F6, G6, H6,
        A7, B7, C7, D7, E7, F7, G7, H7,
        A8, B8, C8, D8, E8, F8, G8, H8,
};

int HasPawnForCapture() {
    int sqWithCanBeCapturedPawn = 0;
    int sidePawn = (side == WHITE) ? P : p;
    if (enpassant != NO_SQUARE) {
        if (side == WHITE) {
            sqWithCanBeCapturedPawn = enpassant + 8;
        } else {
            sqWithCanBeCapturedPawn = enpassant - 8;
        }

        if (getBit(bitboardsPieces[sidePawn], sqWithCanBeCapturedPawn + 1) ||
            (getBit(bitboardsPieces[sidePawn], sqWithCanBeCapturedPawn - 1))) {
            return true;
        }
    }
    return false;
}

U64 polyKeyFromBoard() {
    int square, polyPiece, offset;
    U64 finalKey = 0;

    // hashing pieces
    for (int rank = 0; rank < 8; ++rank) {
        for (int file = 0; file < 8; ++file) {
            square = 8 * rank + file;
            for (int bitPiece = P; bitPiece <= k; ++bitPiece) {
                if (getBit(bitboardsPieces[bitPiece], square)) {
                    polyPiece = PolyTypeOfPiece[bitPiece];
                    finalKey ^= Random64Poly[(64 * polyPiece) + reversedSquares[square]];
                }
            }
        }
    }

    // hashing castling rights
    offset = 768;
    if (castle & WK) {
        finalKey ^= Random64Poly[offset + 0];
    }
    if (castle & WQ) {
        finalKey ^= Random64Poly[offset + 1];
    }
    if (castle & BK) {
        finalKey ^= Random64Poly[offset + 2];
    }
    if (castle & BQ) {
        finalKey ^= Random64Poly[offset + 3];
    }

    offset = 772;
    if (HasPawnForCapture() == true) {
        int file = enpassant % 8;
        finalKey ^= Random64Poly[offset + file];
    }

    if (side == WHITE){
        finalKey ^= Random64Poly[780];
    }

    return finalKey;
}


/**
 * init Entries in book
 */
int initPolyBook(){
    FILE* bookFile = fopen(NAME_OF_BOOK, "rb");
    if(bookFile == nullptr){
        // didn't found book file
        printf("BOOK: BOOK FILE NOT FOUND\n");
        return false;
    }else{
        // measure the size of the book file
        fseek(bookFile,0,SEEK_END);
        long position = ftell(bookFile);
        if(position < sizeof(BookEntry)){
            // no entries found \ file too small
            printf("BOOK: NO ENTRIES FOUND\n");
            return false;
        }
        // reading book entries into allocated memory
        NUM_OF_ENTRIES = position / sizeof(BookEntry);
        printf("book: %ld entries found\n",NUM_OF_ENTRIES);
        entries = (BookEntry*)malloc(NUM_OF_ENTRIES * sizeof(BookEntry));
        rewind(bookFile);
        size_t returnValue = fread(entries, sizeof(BookEntry),NUM_OF_ENTRIES,bookFile);
        printf("book: fread() %ld entries read in from file", returnValue);
        return true;
    }
}

/**
 * free memory of book
 */
void cleanPolyBook(){
    free(entries);
}