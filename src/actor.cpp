#include "actor.h"

#include "assets.h"

namespace boing {

Actor::Actor(std::string image, float x, float y)
    : x(x)
    , y(y)
    , image_(std::move(image))
{
}

void Actor::Draw(Assets& assets) const
{
    assets.BlitCentered(image_, x, y);
}

} // namespace boing