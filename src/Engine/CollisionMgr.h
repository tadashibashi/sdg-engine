/* =============================================================================
 * CollisionMgr
 * 
 * 
 * ===========================================================================*/
#pragma once
#ifndef SDG_COLLISIONMGR_H
#define SDG_COLLISIONMGR_H

#include <unordered_map>
#include <vector>
#include <Engine/Math/Vector2.h>
#include <Engine/Components/Collider.h>
#include <Engine/Components/EntityComponent.h>

namespace SDG
{
    class Entity;

    class CollisionMgr: public EntityComponent {
    public:
        explicit CollisionMgr(Point hashSize = Point(64, 64));
        ~CollisionMgr() override;
        void RegisterCollider(Collider2D *collider);
        void UnregisterCollider(Collider2D *collider);
        void ProcessCollisions();
        void ProcessRemovals();
        void ProcessAdditions();
        [[nodiscard]] Point GetHashSize() const { return hashSize_; }
        void SetHashSize(Point size)
        {
            hashSize_ = size;
        }

    private:
        std::unordered_map<int, std::unordered_map<int, std::vector<Collider2D *>>> colls_;
        std::vector<Collider2D *> colliders_;
        Point hashSize_;
        std::vector<Collider2D *> toAdd_;
        bool toRemove_{false};
    };
}

#endif


