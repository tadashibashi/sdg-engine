/* =============================================================================
 * Zombie
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <Engine/Components/EntityComponent.h>
#include <Engine/Components/SpriteRenderer.h>
#include <Engine/Components/Transform.h>
#include <Engine/Components/Collider.h>
#include <Engine/Math/Rand.h>

namespace SDG
{
    class Zombie : public EntityComponent
    {
    public:
        Zombie(): EntityComponent(true, false),
                   player_(nullptr)
        {}

        void Init() override
        {
            GetComponent<SpriteRenderer>()->SetSpriteByKey("rob-idle");
            player_ = GetSceneMgr()->CurrentScene()->GetFirstEntityWithTag("Player");
            GetComponent<Collider2D>()->SetCallback(OnCollision);
        }

        static void OnCollision(Entity *thiz, Entity *other)
        {
            if (other->GetTag() == "Zombie")
            {
                Vector2 &pos = thiz->Components()->Get<Transform>()->position;
                Vector2 &otherPos = other->Components()->Get<Transform>()->position;
                if (otherPos.x > pos.x) {pos += Vector2(-1, 0);} else {pos += Vector2(1, 0);}
                if (otherPos.y > pos.y) {pos += Vector2(0, -1);} else {pos += Vector2(0, 1);}
            }
            else if (other->GetTag() == "Bullet")
            {
                GetCurrentScene()->DestroyEntity(*thiz);
                GetCurrentScene()->DestroyEntity(*other);
            }
        }

        void Update() override
        {
            if (Rand::Next() < .5)
            {
                if (player_)
                {
                    auto tf = player_->Components()->Get<Transform>();
                    auto mytf = GetComponent<Transform>();

                    mytf->position += (tf->GetPosition() - mytf->GetPosition()).Normalize();
                }
                else
                {
                    player_ = GetSceneMgr()->CurrentScene()->GetFirstEntityWithTag("Player");
                }
            }
            auto spr = this->GetComponent<SpriteRenderer>();
            spr->rotation = std::fmod(spr->rotation + 1, 360);
        }

    private:
        Entity *player_;
        float rotation{};
    };
}