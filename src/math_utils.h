#pragma once

#include <cmath>

#include "raylib.h"

namespace boing {

// Return a unit vector pointing in the same direction as (x, y).
inline Vector2 Normalized(Vector2 value)
{
    const float length = std::sqrt(value.x * value.x + value.y * value.y);
    if (length == 0.0f) {
        return Vector2{0.0f, 0.0f};
    }
    return Vector2{value.x / length, value.y / length};
}

// Returns -1 or 1 depending on whether the value is negative or not.
inline int Sign(float value)
{
    return value < 0.0f ? -1 : 1;
}

} // namespace boing