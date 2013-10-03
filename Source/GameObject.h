#pragma once
#include "PMCommon.h"

namespace irr
{
    namespace scene
    {
        class ISceneNode;
    }

    namespace video
    {
        class ITexture;
    }
}

struct PMVector;

enum GameObjectTexture
{
    TEXTURE_NONE,
    TEXTURE_WIND,
    TEXTURE_PLANE_SHADOW,
    TEXTURE_PLANE_CURSOR,
    TEXTURE_MOVE,
    TEXTURE_RESIZE,
    TEXTURE_SCALE,
    TEXTURE_CLOSE_NORMAL,
    TEXTURE_CLOSE_HOVER,
    TEXTURE_CLOSE_PRESS,
    TEXTURE_CLOSE_DISABLE,
    MAX_TEXTURE,
};

class GameObject
{
public:
    virtual void SetVisible(bool a_result);
    virtual void SetPosition(float a_x, float a_y, float a_z);
    void SetRotation(float a_x, float a_y, float a_z);
    
    void SetScale(PMVector a_scale);
    virtual void SetPosition(PMVector a_position);
    void SetRotation(PMVector a_rotation);

    PMVector GetPosition();
    PMVector GetAbsolutePosition();
    PMVector GetRotation();
    PMVector GetAbsoluteRotation();
    PMVector GetScale();
    PMVector GetAbsoluteScale();

    void SetColor(char a_r, char a_g, char a_b, char a_a);

    void ChangeTexture(GameObjectTexture a_index);

    static void RotationToDirection(PMVector &a_vector, PMVector a_rotation);
    static void SetTexture(GameObjectTexture a_index, irr::video::ITexture* a_texture);

protected:
    friend class GameObjectFactory;
    friend class HandObject;
    friend class AvatarObject;

    GameObject();
    void SetNode(irr::scene::ISceneNode* a_node);
    virtual ~GameObject();

    irr::scene::ISceneNode* m_node;
};