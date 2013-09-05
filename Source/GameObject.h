#pragma once

struct GameVector3D;

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
    GameObject(irr::scene::ISceneNode* a_node);
    ~GameObject();

    void SetVisible(bool a_result);
    void SetPosition(const GameVector3D& a_position);
    void SetRotation(const GameVector3D& a_normal);
};