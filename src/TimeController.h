
#ifndef NISSIMENGINECPP_TIMECONTROLLER_H
#define NISSIMENGINECPP_TIMECONTROLLER_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

#ifdef WIN64

#include <windows.h>

#else
#include <sys/time.h>
#endif


/**
 * flag to see if the GUI \ User send a signal to quit
 */
extern int isQuit;

/**
 * counter for the "movestogo" command
 */
extern int movesToGo;

/**
 * counter for the "movetime" command
 */
extern int moveTime;

/**
 * current time left to play related to UCI parameter "time" command
 */
extern int time;

/**
 * Hold current increment for move
 */
extern int inc;

/**
 * Start time of making the move
 */
extern int startTime;


/**
 * measure the time the calculation needs to stop "stoptime" according to UCI
 */
extern int stopTime;

/**
 * flag to check if there is time control for the current move
 */
extern int isTimeSet;

/**
 * flag to see if the engine need to stop calculation
 */
extern int isStopped;

/**
 * get the current time in MS
 * @return long represents time in milliseconds
 */
int getTimeMs();

/**
 * checking if there is input wating from user or GUI controller
 * @return
 */
int inputWaiting();

/**
 * Reading given input from user \GUI controller and act accordingly
 */
void readInput();

/**
 * check with user \ GUI controller if there are command waiting
 */
static void communicate() {
    // if time is up break here
    if (isTimeSet == 1 && getTimeMs() > stopTime) {
        // tell engine to stop calculating
        isStopped = 1;
    }

    // read GUI input
    readInput();
}
/**
 * reset all Time control/UCI variables
 */
void resetTimeControl();


#endif //NISSIMENGINECPP_TIMECONTROLLER_H
