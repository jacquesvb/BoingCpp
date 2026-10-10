#pragma once

#include <array>
#include <string>
#include <vector>

#include "bat.h"
#include "ball.h"
#include "impact.h"

namespace boing {

class Assets;
class Audio;

// Owns the state of a single match (two bats, the ball, bounce effects and
// scores) and drives all of the gameplay rules.
class Game {
public:
    using ControlFunc = Bat::ControlFunc;

    // Either (or both) control function may be empty, in which case that bat
    // is controlled by the computer (used for the menu's attract mode).
    explicit Game(Audio& audio, ControlFunc p1 = nullptr, ControlFunc p2 = nullptr);

    void Update();
    void Draw(Assets& assets) const;

    // Plays an in-game sound effect. Unless menuSound is true, nothing is
    // played while player 1 is computer-controlled (i.e. in attract mode).
    void PlaySound(const std::string& name, int count = 1, bool menuSound = false);
    void PlayMenuSound(const std::string& name) { PlaySound(name, 1, true); }

    Bat& GetBat(int index) { return bats_[index]; }
    const Bat& GetBat(int index) const { return bats_[index]; }

    const Ball& GetBall() const { return ball_; }

    int GetAiOffset() const { return aiOffset_; }
    void SetAiOffset(int offset) { aiOffset_ = offset; }

    void AddImpact(float x, float y) { impacts_.emplace_back(Vector2{x, y}); }

private:
    Audio& audio_;

    std::array<Bat, 2> bats_;
    Ball ball_;
    std::vector<Impact> impacts_;
    int aiOffset_ = 0;
};

} // namespace boing