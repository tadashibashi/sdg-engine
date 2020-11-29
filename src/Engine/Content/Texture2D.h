/* =============================================================================
 * Texture2D
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <Engine/Core.h>
#include <Engine/Math/Vector2.h>
#include <Engine/GL.h>

namespace SDG
{
    class SDG_API Texture2D
    {
    public:
        Texture2D() : id(0), size() { }
        Texture2D(unsigned int id, int width, int height)
            : id(id), size(width, height) { }
        [[nodiscard]] GLuint GetID() const { return id; }
        [[nodiscard]] Point GetSize() const { return size; }
        [[nodiscard]] int GetWidth() const { return size.w; }
        [[nodiscard]] int GetHeight() const { return size.h; }

        bool operator==(const Texture2D &other) const { return id == other.id; }
        [[nodiscard]] bool IsLoaded() const { return id != 0; }
        int *SizeData() { return &size.x; }
    private:
        // GL Texture ID
        GLuint id;
        // Pixel dimensions
        Point size;
    };
}