#include "PMType.h"

PMVector operator+(PMVector a_v1, PMVector a_v2)
{
    return PMVector(a_v1.x + a_v2.x, a_v1.y + a_v2.y, a_v1.z + a_v2.z);
}

PMVector operator-(PMVector a_v1, PMVector a_v2)
{
    return PMVector(a_v1.x - a_v2.x, a_v1.y - a_v2.y, a_v1.z - a_v2.z);
}

PMVector operator*( PMVector a_v, float a_value )
{
    return PMVector(a_v.x * a_value, a_v.y * a_value, a_v.z * a_value);
}

PMVector operator*( float a_value, PMVector a_v )
{
    return a_v * a_value;
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


