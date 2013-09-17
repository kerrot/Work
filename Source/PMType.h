#pragma once

struct PMVector
{
    PMVector()
    :
    x(0)
    ,y(0)
    ,z(0)
    {

    }

    PMVector(float a_x, float a_y, float a_z)
    :
    x(a_x)
    ,y(a_y)
    ,z(a_z)
    {

    }

    float Dot(PMVector a_v)
    {
        return x * a_v.x + y * a_v.y + z * a_v.z;
    }

    PMVector CrossProduct(PMVector a_v)
    {
        return PMVector(y * a_v.z - z * a_v.y, z * a_v.x - x * a_v.z, x * a_v.y - y * a_v.x);
    }

    float MagnitudeSquared()
    {
        return x * x + y * y + z * z;
    }

    float x;
    float y;
    float z;
};

PMVector operator+(PMVector a_v1, PMVector a_v2);
PMVector operator-(PMVector a_v1, PMVector a_v2);
PMVector operator*(PMVector a_v, float a_value);
PMVector operator*(float a_value, PMVector a_v);
void operator+=(PMVector &a_v1, PMVector a_v2);
void operator-=(PMVector &a_v1, PMVector a_v2);