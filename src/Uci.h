/**
 * UCI command parser, in charge of parsing board situation, and sending search results
 */

#ifndef NISSIMENGINECPP_UCI_H
#define NISSIMENGINECPP_UCI_H

// system headers
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include "NegaMaxSearch.h"

// FEN dedug positions
#define empty_board "8/8/8/8/8/8/8/8 b - - "
#define start_position "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 "
#define tricky_position "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 "
#define killer_position "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"
#define cmk_position "r2q1rk1/ppp2ppp/2n1bn2/2b1p3/3pP3/3P1NPP/PPP1NPB1/R1BQ1RK1 b - - 0 9 "
#define repetitions "2r3k1/R7/8/1R6/8/8/P4KPP/8 w - - 0 40 "

#define MAX_HASH_SIZE 128
#define MIN_HASH_SIZE 4
#define DEFAULT_HASH_SIZE 64

#define ENGINE_NAME "NiCim"
#define VERSION "3.2.1"


/**
 * Parsing a given move as string to the proper encoded move as Integer
 * @param moveAsString String that represents the move ("e2e4" or "a7a8q")
 * @return Integer represents the proper encoded move
 */
int parseMove(const char *moveAsString);

/**
 * Parsing the given uci command and change the board data accordingly
 * @param command String represents uci command to change the board
 */
void parsePosition(char *command);


/**
 * Initializing search according to time control, depth, etc
 * @param command
 */
void parseGo(char *command);

/**
 * Main UCI loop for communicating with GUI UCI
 */
void UCILoop();


#endif //NISSIMENGINECPP_UCI_H



