#include "GamePhysics.h"
#include "GameObjectFactory.h"
#include "CollidableObject.h"
#include "LeafObject.h"

void GamePhysics::PhysicsUpdate()
{
    std::vector<LeafObject*>& leaves = sGameObjectFactory.GetLeaves();
    std::vector<CollidableObject*>& collidables = sGameObjectFactory.GetCollidables();

    for (std::vector<CollidableObject*>::iterator cIter = collidables.begin();
        cIter != collidables.end();
        ++cIter)
    {
        for (std::vector<LeafObject*>::iterator lIter = leaves.begin();
            lIter != leaves.end();
            ++lIter)
        {
            CollidableObject* collidable = *cIter;
            LeafObject* leaf = *lIter;
            collidable->HitObject(leaf);
        }
    }

    for (std::vector<LeafObject*>::iterator lIter = leaves.begin();
        lIter != leaves.end();
        ++lIter)
    {
        LeafObject* leaf = *lIter;
        leaf->UpdatePosition();
    }
}
