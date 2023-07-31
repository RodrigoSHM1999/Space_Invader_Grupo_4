#include <cstdlib>
#include "misc.h"

unsigned Misc::random(const unsigned low, const unsigned high)
{
    return (rand() % (high - low + 1)) + low;// establecer el rango
}
