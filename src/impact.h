#pragma once

#include "actor.h"

namespace boing {

// A brief animation shown at the spot where the ball bounces off a bat or a
// wall. Its frame advances every two updates; once its time reaches 10 the
// owning Game removes it.
class Impact : public Actor {
public:
    explicit Impact(Vector2 pos);

    int Time() const { return time_; }

    void Update();

private:
    int time_ = 0;
};

} // namespace boing