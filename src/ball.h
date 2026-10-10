#pragma once

#include "actor.h"
#include "constants.h"

namespace boing {

class Game;

// The ball. dx/dy form a unit vector describing its direction, while speed
// controls how many sub-steps it moves each frame (and increases with every
// bat hit).
class Ball : public Actor {
public:
    explicit Ball(float dx);

    // True once the ball has left the left or right edge of the screen.
    bool Out() const { return x < 0.0f || x > static_cast<float>(WIDTH); }

    void Update(Game& game);

private:
    float dx_ = 0.0f;
    float dy_ = 0.0f;
    int speed_ = 5;
};

} // namespace boing