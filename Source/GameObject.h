#pragma once

namespace irr
{
    namespace scene
    {
        class ISceneNode;
    }
}

class GameObject
{
public:
    void SetVisible(bool a_result);
    void SetPosition(float a_x, float a_y, float a_z);
    void SetRotation(float a_x, float a_y, float a_z);

protected:
    friend class GameObjectFactory;
    GameObject(irr::scene::ISceneNode* a_node);
    virtual ~GameObject();

    irr::scene::ISceneNode* m_node;
};