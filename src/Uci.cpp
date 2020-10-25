
#include "Uci.h"

/**
 * Parsing a given move as string to the proper encoded move as Integer
 * @param moveAsString String that represents the move ("e2e4" or "a7a8q")
 * @return Integer represents the proper encoded move
 */
int parseMove(const char *moveAsString) {
    moves moveList[1];
    generateMoves(moveList);

    // parsing source and target squares
    int sourceSquare = (moveAsString[0] - 'a') + (8 - (moveAsString[1] - '0')) * 8;
    int targetSquare = (moveAsString[2] - 'a') + (8 - (moveAsString[3] - '0')) * 8;


    for (int i = 0; i < moveList->count; i++) {
        // for each of the possible moves, check if it matches the given move
        int move = moveList->moves[i];

        if (sourceSquare == getMoveSource(move) && targetSquare == getMoveTarget(move)) {
            // check for promotion piece
            int promotedPiece = getMovePromoted(move);

            // promoted piece is available
            if (promotedPiece) {

                if ((promotedPiece == Q || promotedPiece == q) && moveAsString[4] == 'q') {
                    return move;
                } else if ((promotedPiece == R || promotedPiece == r) && moveAsString[4] == 'r') {
                    return move;
                } else if ((promotedPiece == B || promotedPiece == b) && moveAsString[4] == 'b') {
                    return move;
                } else if ((promotedPiece == N || promotedPiece == n) && moveAsString[4] == 'n') {
                    return move;
                }


                // continue the loop on possible wrong promotions (e.g. "e7e8f")
                continue;
            }

            // return legal move
            return move;
        }
    }

    // return illegal move
    return 0;
}

/**
 * Parsing the given uci command and change the board data accordingly
 * @param command String represents uci command to change the board
 */
void parsePosition(char *command) {
    // shift pointer to the right where next token begins
    command += 9;

    // init pointer to the current character in the command string
    char *currentChar = command;


    if (strncmp(command, "startpos", 8) == 0) {
        //parse UCI "startpos" commandinit chess board with start position
        parseFen(start_position);
    }

        //
    else {
        // parse UCI "fen" command make sure "fen" command is available within command string
        currentChar = strstr(command, "fen");

        if (currentChar == nullptr)
            // no fen, init chess board with start position
            parseFen(start_position);
        else {
            // skip "fen " and parse
            currentChar += 4;

            parseFen(currentChar);
        }
    }

    currentChar = strstr(command, "moves");

    if (currentChar != nullptr) {
        //there are moves available, skip "moves "
        currentChar += 6;

        while (*currentChar) {
            // parse each move
            int move = parseMove(currentChar);

            // if no more moves
            if (move == 0) {
                //invalid move. break
                break;
            }

            // updating repetition table
            repetitionIndex++;
            repetitionTable[repetitionIndex] = hashKey;


            makeMove(move, ALL_MOVES);

            // go to next move, consider promotion moves(with letter instead of space)
            while (*currentChar && *currentChar != ' ') {
                currentChar++;
            }
            currentChar++;
        }
    }
    printBoard();
}


/**
 * Initializing search according to time control, depth, etc
 * @param command
 */
void parseGo(char *command) {
    resetTimeControl();

    // init parameters
    int depth = -1;
    char *argument = nullptr;


    if ((argument = strstr(command, "infinite"))) {
        // infinite search nothing to do here
    }

    if ((argument = strstr(command, "binc")) && side == BLACK) {
        //  match UCI "binc" command parse BLACK time increment
        increment = atoi(argument + 5);
    }
    if ((argument = strstr(command, "winc")) && side == WHITE) {
        // match UCI "winc" command parse WHITE time increment
        increment = atoi(argument + 5);
    }

    if ((argument = strstr(command, "wtime")) && side == WHITE) {
        // match UCI "wtime" command parse WHITE time limit
        time = atoi(argument + 6);
    }


    if ((argument = strstr(command, "btime")) && side == BLACK) {
        // match UCI "btime" command parse BLACK time limit
        time = atoi(argument + 6);
    }

    if ((argument = strstr(command, "movestogo"))) {
        // match UCI "movestogo" command parse number of moves to go
        movesToGo = atoi(argument + 10);
    }

    if ((argument = strstr(command, "movetime"))) {
        // match UCI "moveTime" command parse amount of time allowed to spend to make a move
        moveTime = atoi(argument + 9);
    }


    if ((argument = strstr(command, "depth"))) {
        // match UCI "depth" command parse search depth
        depth = atoi(argument + 6);
    }


    if (moveTime != -1) {
        // if move time is not available set time equal to move time
        time = moveTime;
        movesToGo = 1;
    }

    startTime = getTimeMs();


    if (time != -1) {
        // if time control is available flag we're playing with time control
        isTimeSet = 1;

        // set up timing
        time /= movesToGo;
        time -= 50;

        if(time < 0){
            time = 0;
            increment -=50;
            if(increment < 0){
                increment = 1;
            }
        }


        // init stoptime
        stopTime = startTime + time + increment;
    }


    if (depth == -1) {
        // depth not available -> set depth to 64 plies
        depth = 64;
    }


    // print debug info
    printf("time: %ld inc:%ld  start: %ld  stop: %ld  depth: %d  timeset:%d\n",
           time, increment, startTime, stopTime, depth, isTimeSet);


    searchPosition(depth);
}

/**
 * Print info about the engine, and declares it is working with UCI protocol
 * @param withHash Integer flag to point if to print Hash data, or not.( withHash=0 ->not printing, withHash!=0 -> printing)
 */
void printInfo(int withHash) {
    // print engine info
    printf("id name %s %s\n", ENGINE_NAME, VERSION);
    printf("id author yodatk\n");
    if (withHash) {
        printf("option name Hash type spin default %d min %d max %d\n", DEFAULT_HASH_SIZE, MIN_HASH_SIZE,
               MAX_HASH_SIZE);
    }
    printf("uciok\n");
}

/**
 * Main UCI loop for communicating with GUI UCI
 */
void UCILoop() {

    int mb = DEFAULT_HASH_SIZE;

    // reset STDIN & STDOUT buffers and setting input buffer
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    char input[2000];

    printInfo(1);

    // main loop
    while (true) {
        // reset buffers
        memset(input, 0, sizeof(input));
        fflush(stdout);


        if (!fgets(input, 2000, stdin) || (input[0] == '\n')) {
            // no input
            continue;
        }


        if (strncmp(input, "isready", 7) == 0) {
            // parse UCI "isready" command
            printf("readyok\n");
            continue;
        } else if (strncmp(input, "position", 8) == 0) {
            // parse UCI "position" command call parse position function
            parsePosition(input);
            clearHashTable();
        } else if (strncmp(input, "ucinewgame", 10) == 0) {
            // parse UCI "ucinewgame" command call parse position function
            parsePosition("position startpos");
            clearHashTable();
        } else if (strncmp(input, "go", 2) == 0) {
            // parse UCI "go" command
            parseGo(input);
        } else if (strncmp(input, "quit", 4) == 0) {
            // parse UCI "quit" command quit from the UCI loop (terminate program)
            break;
        } else if (strncmp(input, "uci", 3) == 0) {
            // parse UCI "uci" command -> printing info without hash info
            printInfo(0);
        } else if (!strncmp(input, "setoption name Hash value ", 26)) {
            // init MB
            sscanf(input, "%*s %*s %*s %*s %d", &mb);

            // adjust MB if going beyond the aloowed bounds
            if (mb < MIN_HASH_SIZE) {
                mb = MIN_HASH_SIZE;
            }
            if (mb > MAX_HASH_SIZE) {
                mb = MAX_HASH_SIZE;
            }
            // set hash table size in MB
            printf("    Set hash table size to %dMB\n", mb);
            initHashTable(mb);
        }
    }
}
