#include "mathutils.h"

bool max::inRange(int low, int high, int x)
{
    return ((unsigned)(x - low) <= (high - low));
}