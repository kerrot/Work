#ifndef __GAME_MATRIX_4_H__
#define __GAME_MATRIX_4_H__

#include <matrix4.h>
#include <OVR.h>

class GameMatrix4
{
public:
    GameMatrix4();

    GameMatrix4(const irr::core::matrix4 &matrix);

    GameMatrix4(const OVR::Matrix4f &matrix);

    float& operator[](uint index) { return m[index]; }

    operator irr::core::matrix4() const;

    operator OVR::Matrix4f() const;

    void switchHanded();

private:
    float m[16];
};

#endif
