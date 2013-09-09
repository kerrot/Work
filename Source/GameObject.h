#pragma once

namespace irr
{
    namespace scene
    {
        class ISceneNode;
    }
}

struct PMVector;

class GameObject
{
public:
    void SetVisible(bool a_result) const;
    void SetPosition(float a_x, float a_y, float a_z) const;
    void SetRotation(float a_x, float a_y, float a_z) const;
    
    void SetScale(PMVector a_scale) const;
    void SetPosition(PMVector a_position) const;
    void SetRotation(PMVector a_rotation) const;

    PMVector GetPosition() const;

protected:
    friend class GameObjectFactory;
    GameObject();
    void SetNode(irr::scene::ISceneNode* a_node);
    virtual ~GameObject();

    irr::scene::ISceneNode* m_node;
};