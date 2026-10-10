#pragma once

#include <string>

#include "raylib.h"

namespace boing {

class Assets;

// Base class for all in-game drawable entities.
//
// Like a Pygame Zero Actor, an entity tracks a position (its centre) and the
// name of the image used to draw it. Images are drawn centred on the position.
class Actor {
public:
    Actor(std::string image, float x, float y);
    virtual ~Actor() = default;

    // Centre position of the actor.
    float x = 0.0f;
    float y = 0.0f;

    const std::string& Image() const { return image_; }
    void SetImage(std::string image) { image_ = std::move(image); }

    // Draws the image centred on (x, y).
    void Draw(Assets& assets) const;

protected:
    std::string image_;
};

} // namespace boing