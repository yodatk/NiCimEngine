
#include "TimeController.h"


/**
 * flag to see if the GUI \ User send a signal to quit
 */
int isQuit = 0;

/**
 * counter for the "movestogo" command
 */
int movesToGo = 30;


/**
 * counter for the "movetime" command
 */
int moveTime = -1;

/**
 * current time left to play related to UCI parameter "time" command
 */
int time = -1;


/**
 * Hold current increment for move
 */
int inc = 0;

/**
 * Start time of making the move
 */
int startTime = 0;

/**
 * measure the time the calculation needs to stop "stoptime" according to UCI
 */
int stopTime = 0;

/**
 * flag to check if there is time control for the current move
 */
int isTimeSet = 0;


/**
 * flag to see if the engine need to stop calculation
 */
int isStopped = 0;

// Miscellaneous functions forked from VICE by Richard Allbert

/**
 * get the current time in MS
 * @return long represents time in milliseconds
 */
int getTimeMs() {
#ifdef WIN64
    return GetTickCount();
#else
    struct timeval timeValue;
    gettimeofday(&timeValue, NULL);
    return timeValue.tv_sec * 1000 + timeValue.tv_usec / 1000;
#endif
}

/**
 * checking if there is input wating from user or GUI controller
 * @return flag to deteremine if there is input or not
 */
int inputWaiting() {
#ifndef WIN32
    fd_set readfds;
    struct timeval tv;
    FD_ZERO(&readfds);
    FD_SET(fileno(stdin), &readfds);
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    select(16, &readfds, 0, 0, &tv);

    return (FD_ISSET(fileno(stdin), &readfds));
#else
    static int init = 0, pipe;
    static HANDLE inh;
    DWORD dw;

    if (!init) {
        init = 1;
        inh = GetStdHandle(STD_INPUT_HANDLE);
        pipe = !GetConsoleMode(inh, &dw);
        if (!pipe) {
            SetConsoleMode(inh, dw & ~(ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT));
            FlushConsoleInputBuffer(inh);
        }
    }

    if (pipe) {
        if (!PeekNamedPipe(inh, NULL, 0, NULL, &dw, NULL))
            return 1;
        return dw;
    } else {
        GetNumberOfConsoleInputEvents(inh, &dw);
        return dw <= 1 ? 0 : dw;
    }

#endif
}

/**
 * Reading given input from user \GUI controller and act accordingly
 */
void readInput() {
    // bytes to read holder
    int bytes;

    // GUI/user input
    char input[256] = "", *endc;

    // "listen" to STDIN
    if (inputWaiting()) {
        // tell engine to stop calculating
        isStopped = 1;

        // loop to read bytes from STDIN
        do {
            // read bytes from STDIN
            bytes = read(fileno(stdin), input, 256);
        }

            // until bytes available
        while (bytes < 0);

        // searches for the first occurrence of '\n'
        endc = strchr(input, '\n');

        // if found new line set value at pointer to 0
        if (endc)
            *endc = 0;

        // if input is available
        if (strlen(input) > 0) {
            // match UCI "quit" command
            if (!strncmp(input, "quit", 4))
                // tell engine to terminate exacution
                isQuit = 1;

                // // match UCI "stop" command
            else if (!strncmp(input, "stop", 4))
                // tell engine to terminate exacution
                isQuit = 1;
        }
    }
}





/**
 * reset all Time control/UCI variables
 */
void resetTimeControl() {
    // reset timing
    isQuit = 0;
    movesToGo = 30;
    moveTime = -1;
    time = -1;
    inc = 0;
    startTime = 0;
    stopTime = 0;
    isTimeSet = 0;
    isStopped = 0;
}