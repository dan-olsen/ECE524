#ifndef PATHSET
#define PATHSET

#include "given.h"

typedef struct PATH_type
{
    LIST *Path;
    int suspect;

} PATH;

typedef struct PATH_SET_type {
    int Id;
    int numLongestPath;
    int numSecondLongestPath;
    PATH *longestPath;
    PATH *secondLongestPath;

} PATH_SET;

PATH_SET *pathSet;

#endif // PATHSET

