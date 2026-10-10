#pragma once

#include <functional>

#include "actor.h"

namespace boing {

class Game;

// A bat: either player controlled or driven by the AI. Each player has three
// sprites: standard, "just bounced off me", and "I just missed the ball".
class Bat : public Actor {
public:
    // Returns how far the bat should move this frame (negative = up).
    using ControlFunc = std::function<float()>;

    Bat(int player, ControlFunc control = nullptr);

    int Score() const { return score_; }
    void AddScore() { ++score_; }

    int Timer() const { return timer_; }
    void SetTimer(int timer) { timer_ = timer; }

    // True when this bat is driven by a human player rather than the AI.
    bool IsHuman() const { return static_cast<bool>(control_); }

    void Update(Game& game);

private:
    // How far the computer player wants to move this frame.
    float AiMove(Game& game) const;

    int player_;
    int score_ = 0;
    int timer_ = 0;
    ControlFunc control_;
};

// Control functions for the two human players.
float P1Controls();
float P2Controls();

} // namespace boing