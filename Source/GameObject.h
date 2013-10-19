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
    TEXTURE_START_NORMAL,
    TEXTURE_START_HOVER,
    TEXTURE_START_PRESS,
    TEXTURE_END_NORMAL,
    TEXTURE_END_HOVER,
    TEXTURE_END_PRESS,
    TEXTURE_COMIC_NORMAL,
    TEXTURE_COMIC_HOVER,
    TEXTURE_COMIC_PRESS,
    TEXTURE_MENU_BG,
    TEXTURE_NEXT_NORMAL,
    TEXTURE_NEXT_HOVER,
    TEXTURE_NEXT_PRESS,
    TEXTURE_NEXT_DISABLE,
    TEXTURE_LAST_NORMAL,
    TEXTURE_LAST_HOVER,
    TEXTURE_LAST_PRESS,
    TEXTURE_LAST_DISABLE,
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

    bool IsVisible();

    PMVector GetPosition();
    PMVector GetAbsolutePosition();
    PMVector GetRotation();
    PMVector GetAbsoluteRotation();
    PMVector GetScale();

    void SetColor(char a_r, char a_g, char a_b, char a_a);

    void RecoverTexture();
    void ChangeTexture(GameObjectTexture a_index);
    void SetParent(GameObject* a_object);

    static void RotationToDirection(PMVector &a_vector, PMVector a_rotation);
    static void SetTexture(GameObjectTexture a_index, irr::video::ITexture* a_texture);
    static irr::video::ITexture* GetTexture(GameObjectTexture a_index);

protected:
    friend class GameObjectFactory;
    friend class HandObject;
    friend class AvatarObject;

    GameObject();
    void SetNode(irr::scene::ISceneNode* a_node);
    virtual ~GameObject();

    GameObjectTexture m_textureID;
    irr::scene::ISceneNode* m_node;
    irr::video::ITexture* m_renderTarget;
};