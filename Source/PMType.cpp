#include "PMType.h"

PMVector operator+(PMVector a_v1, PMVector a_v2)
{
    return PMVector(a_v1.x + a_v2.x, a_v1.y + a_v2.y, a_v1.z + a_v2.z);
}

PMVector operator-(PMVector a_v1, PMVector a_v2)
{
    return PMVector(a_v1.x - a_v2.x, a_v1.y - a_v2.y, a_v1.z - a_v2.z);
}

void operator+=(PMVector &a_v1, PMVector a_v2)
{
    a_v1.x += a_v2.x;
    a_v1.y += a_v2.y;
    a_v1.z += a_v2.z;
}

void operator-=(PMVector &a_v1, PMVector a_v2)
{
    a_v1.x -= a_v2.x;
    a_v1.y -= a_v2.y;
    a_v1.z -= a_v2.z;
}