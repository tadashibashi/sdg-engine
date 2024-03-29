/* =============================================================================
 * SwapEndian
 * Template function for swapping data endianness
 * ===========================================================================*/
#pragma once
#include <climits>
namespace SDG
{
    template <typename T>
    T SwapEndian(T u)
    {
        static_assert (CHAR_BIT == 8, "CHAR_BIT != 8");

        union
        {
            T u;
            unsigned char u8[sizeof(T)];
        } source, dest;

        source.u = u;

        for (size_t k = 0; k < sizeof(T); k++)
            dest.u8[k] = source.u8[sizeof(T) - k - 1];

        return dest.u;
    }
}