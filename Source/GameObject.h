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

    void SetColor(char a_r, char a_g, char a_b, char a_a);

    static void RotationToDirection(PMVector &a_vector, PMVector a_rotation);

protected:
    friend class GameObjectFactory;
    friend class HandObject;
    friend class AvatarObject;
    
    GameObject();
    void SetNode(irr::scene::ISceneNode* a_node);
    virtual ~GameObject();

    irr::scene::ISceneNode* m_node;
};