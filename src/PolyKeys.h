/**
 * hash keys for reading from opening book
 */

#ifndef NICIMENGINE_POLYKEYS_H
#define NICIMENGINE_POLYKEYS_H



#ifdef _MSC_VER
#  define U64_POLY(u) (u##ui64)
#else
#  define U64_POLY(u) (u##ULL)
#endif

#include "Board.h"

extern const U64 Random64Poly[781];


#endif //NICIMENGINE_POLYKEYS_H
