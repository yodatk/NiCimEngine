

#include "PolyBook.h"
#include "Uci.h"

/**
 * boolean flag to determine if to use book or not
 */
int IS_USING_BOOK = true;

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
BookEntry *entries;

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

    if (side == WHITE) {
        finalKey ^= Random64Poly[780];
    }

    return finalKey;
}


/**
 * init Entries in book
 */
int initPolyBook() {
    FILE *bookFile = fopen(NAME_OF_BOOK, "rb");
    if (bookFile == nullptr) {
        // didn't found book file
        printf("BOOK: BOOK FILE NOT FOUND\n");
        return false;
    } else {
        // measure the size of the book file
        fseek(bookFile, 0, SEEK_END);
        long position = ftell(bookFile);
        if (position < sizeof(BookEntry)) {
            // no entries found \ file too small
            printf("BOOK: NO ENTRIES FOUND\n");
            return false;
        }
        // reading book entries into allocated memory
        NUM_OF_ENTRIES = position / sizeof(BookEntry);
        if (NUM_OF_ENTRIES <= 0) {
            // no entries
            return false;
        }
        printf("book: %ld entries found\n", NUM_OF_ENTRIES);
        entries = (BookEntry *) malloc(NUM_OF_ENTRIES * sizeof(BookEntry));
        rewind(bookFile);
        size_t returnValue = fread(entries, sizeof(BookEntry), NUM_OF_ENTRIES, bookFile);
        printf("book: read %llu entries read in from file", returnValue);

        return true;
    }
}

unsigned short endianSwapU16(unsigned short x) {
    x = (x >> 8) | (x << 8);
    return x;
}

unsigned int endianSwapU32(unsigned int x) {
    x = (x >> 24) |
        ((x << 8) & 0x00FF0000) |
        ((x >> 8) & 0x0000FF00) |
        (x << 24);
    return x;
}

U64 endianSwapU64(U64 x) {
    x =
            (x >> 56) |
            ((x << 40) & 0x00FF000000000000) |
            ((x << 24) & 0x0000FF0000000000) |
            ((x << 8) & 0x000000FF00000000) |
            ((x >> 8) & 0x00000000FF000000) |
            ((x >> 24) & 0x0000000000FF0000) |
            ((x >> 40) & 0x000000000000FF00) |
            (x << 56);
    return x;
}

/**
 * free memory of book
 */
void cleanPolyBook() {
    free(entries);
}


int convertPolyMoveToNiCimMove(unsigned short move) {
    short sourceFile = (move >> 6) & 7;
    short sourceRank = (move >> 9) & 7;
    short sourceSquare = reversedSquares[8 * sourceRank + sourceFile];
    short targetFile = (move >> 0) & 7;
    short targetRank = (move >> 3) & 7;
    short targetSquare = reversedSquares[8 * targetRank + targetFile];
    short promotedPiece = (move >> 12) & 7;
    char moveString[6];
    if (promotedPiece == 0) {
        sprintf(moveString, "%s%s", squareToCoordinates[sourceSquare], squareToCoordinates[targetSquare]);
    } else {
        char promotionChar = 'q';
        switch (promotedPiece) {
            case 1 :
                promotionChar = 'n';
                break;
            case 2 :
                promotionChar = 'b';
                break;
            case 3 :
                promotionChar = 'r';
                break;
        }
        sprintf(moveString, "%s%s%c", squareToCoordinates[sourceSquare], squareToCoordinates[targetSquare],
                promotionChar);
    }
    return parseMove(moveString);
}

/**
 * listing all the moves option from the given polykey
 * @param polkey U64 number which is a hashed move key
 */
int getBookMove() {
    BookEntry *entry;
    unsigned short move;
    const int MAX_BOOKS_MOVES = 32;
    int bookMoves[MAX_BOOKS_MOVES];
    int tempMove = 0;
    int count = 0;
    U64 polykey = polyKeyFromBoard();

    for (entry = entries; entry < entries + NUM_OF_ENTRIES; entry++) {
        if (polykey == endianSwapU64(entry->key)) {
            move = endianSwapU16(entry->move);
            tempMove = convertPolyMoveToNiCimMove(move);
            if (tempMove != 0) {
                bookMoves[count++] = tempMove;
                if (count > MAX_BOOKS_MOVES) {
                    break;
                }
            }
        }
    }
    // randomly selecting a move
    if (count > 0) {
        int randMove = rand() % count;
        return bookMoves[randMove];
    } else {
        return 0;
    }

}
