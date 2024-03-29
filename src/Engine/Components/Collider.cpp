/* =============================================================================
 * Collider
 * 
 * 
 * ===========================================================================*/
#include "Collider.h"
#include <Engine/CollisionMgr.h>

namespace SDG
{
    void Collider2D::Init()
    {
        body_ = GetComponent<Body>();
        GetScene()->GetCollisions()->RegisterCollider(this);
    }

    void Collider2D::Close()
    {
        GetScene()->GetCollisions()->UnregisterCollider(this);
    }

    void Collider2D::Draw()
    {
        if (show)
        {
            // Draw debug rectangle
            GetSpriteBatch()->DrawRectangle((FRectangle)body_->GetBounds(),
                Color(127, 255, 127, 50), -10000.f);
        }
    }

    bool Collider2D::CheckCollision(Collider2D *other)
    {
        SDG_ASSERT(other->body_);
        SDG_ASSERT(body_);

        return other->body_->GetBounds().Intersects(body_->GetBounds());
    }


    void Collider2D::Update()
    {

    }
}