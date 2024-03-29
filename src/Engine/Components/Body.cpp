/* =============================================================================
 * Body
 * 
 * 
 * ===========================================================================*/
#include "Body.h"
#include "SpriteRenderer.h"

namespace SDG
{
    void Body::Update()
    {
        float x = 0, y = 0;
        if (useSpriteMask && spr)
        {
            const Sprite *s;
            if ((s = spr->GetSprite()))
            {
                const Frame &f = spr->GetCurrentFrame();
                Rectangle maskRect = s->GetMask();
                size.w = (float)maskRect.w;
                size.h = (float)maskRect.h;

                float pivX, pivY;
                if (f.hasPivot)
                {
                    pivX = f.pivX;
                    pivY = f.pivY;
                }
                else
                {
                    pivX = s->GetOffset().x;
                    pivY = s->GetOffset().y;
                }

                x -= pivX * (float)f.ow - (float)maskRect.x;
                y -= pivY * (float)f.oh - (float)maskRect.y;
            }
        }

        // Velocity
        tf->position += this->velocity;
        position = Vector2(x, y) + tf->GetPosition();
    }

    void Body::Draw()
    {
        if (show)
        {
            FRectangle rect(position.x, position.y, size.w, size.h);
            GetSpriteBatch()->DrawRectangle(rect, Color(127, 255, 127, 80), 0);
        }
    }

    void Body::Init()
    {
        tf = GetComponent<Transform>();
        spr = GetComponent<SpriteRenderer>();
        Update();
    }
}