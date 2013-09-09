#include "GameMatrix4.h"

GameMatrix4::GameMatrix4()
{
    m[ 0] = 0.0f;
    m[ 1] = 0.0f;
    m[ 2] = 0.0f;
    m[ 3] = 0.0f;
    m[ 4] = 0.0f;
    m[ 5] = 0.0f;
    m[ 6] = 0.0f;
    m[ 7] = 0.0f;
    m[ 8] = 0.0f;
    m[ 9] = 0.0f;
    m[10] = 0.0f;
    m[11] = 0.0f;
    m[12] = 0.0f;
    m[13] = 0.0f;
    m[14] = 0.0f;
    m[15] = 0.0f;
}

GameMatrix4::GameMatrix4(const irr::core::matrix4& matrix)
{
    m[ 0] = matrix[ 0];
    m[ 1] = matrix[ 1];
    m[ 2] = matrix[ 2];
    m[ 3] = matrix[ 3];
    m[ 4] = matrix[ 4];
    m[ 5] = matrix[ 5];
    m[ 6] = matrix[ 6];
    m[ 7] = matrix[ 7];
    m[ 8] = matrix[ 8];
    m[ 9] = matrix[ 9];
    m[10] = matrix[10];
    m[11] = matrix[11];
    m[12] = matrix[12];
    m[13] = matrix[13];
    m[14] = matrix[14];
    m[15] = matrix[15];
}

GameMatrix4::GameMatrix4(const OVR::Matrix4f& matrix)
{
    m[ 0] = matrix.M[0][0];
    m[ 1] = matrix.M[1][0];
    m[ 2] = matrix.M[2][0];
    m[ 3] = matrix.M[3][0];
    m[ 4] = matrix.M[0][1];
    m[ 5] = matrix.M[1][1];
    m[ 6] = matrix.M[2][1];
    m[ 7] = matrix.M[3][1];
    m[ 8] = matrix.M[0][2];
    m[ 9] = matrix.M[1][2];
    m[10] = matrix.M[2][2];
    m[11] = matrix.M[3][2];
    m[12] = matrix.M[0][3];
    m[13] = matrix.M[1][3];
    m[14] = matrix.M[2][3];
    m[15] = matrix.M[3][3];
}

GameMatrix4::operator irr::core::matrix4() const
{
    irr::core::matrix4 ret;

    ret[ 0] = m[ 0];
    ret[ 1] = m[ 1];
    ret[ 2] = m[ 2];
    ret[ 3] = m[ 3];
    ret[ 4] = m[ 4];
    ret[ 5] = m[ 5];
    ret[ 6] = m[ 6];
    ret[ 7] = m[ 7];
    ret[ 8] = m[ 8];
    ret[ 9] = m[ 9];
    ret[10] = m[10];
    ret[11] = m[11];
    ret[12] = m[12];
    ret[13] = m[13];
    ret[14] = m[14];
    ret[15] = m[15];

    return ret;
}

GameMatrix4::operator OVR::Matrix4f() const
{
    return OVR::Matrix4f(
        m[ 0], m[ 4], m[ 8], m[12],
        m[ 1], m[ 5], m[ 9], m[13],
        m[ 2], m[ 6], m[10], m[14],
        m[ 3], m[ 7], m[11], m[15]
    );
}

void GameMatrix4::switchHanded()
{
    m[ 8] = -m[ 8];
    m[ 9] = -m[ 9];
    m[10] = -m[10];
    m[11] = -m[11];
}
