#include "impact.h"

#include <string>

namespace boing {

Impact::Impact(Vector2 pos)
    : Actor("blank", pos.x, pos.y)
{
}

void Impact::Update()
{
    // There are 5 impact sprites numbered 0 to 4; switch every 2 frames.
    SetImage("impact" + std::to_string(time_ / 2));
    ++time_;
}

} // namespace boing