#include "bat.h"

#include <algorithm>
#include <cmath>
#include <string>

#include "constants.h"
#include "game.h"

namespace boing {

Bat::Bat(int player, ControlFunc control)
    : Actor("blank", player == 0 ? 40.0f : 760.0f, static_cast<float>(HALF_HEIGHT)),
      player_(player),
      control_(std::move(control))
{
}

void Bat::Update(Game& game)
{
    --timer_;

    // The control function tells us how far to move on the Y axis.
    const float movement = control_ ? control_() : AiMove(game);

    // Apply the movement, keeping the bat inside the walls.
    y = std::min(BAT_MAX_Y, std::max(BAT_MIN_Y, y + movement));

    // Pick the sprite: 0 is normal, 1 is used just after a bounce and 2 just
    // after the ball went out of bounds.
    int frame = 0;
    if (timer_ > 0) {
        frame = game.GetBall().Out() ? 2 : 1;
    }

    SetImage("bat" + std::to_string(player_) + std::to_string(frame));
}

float Bat::AiMove(Game& game) const
{
    // Decide where the AI wants to be. It aims for the centre of the screen
    // until the ball gets close, then tracks the ball's Y position (with a
    // small offset so it does not always hit dead centre).
    const float xDistance = std::fabs(game.GetBall().x - x);
    const float targetY1 = static_cast<float>(HALF_HEIGHT);
    const float targetY2 = game.GetBall().y + static_cast<float>(game.GetAiOffset());

    const float weight1 = std::min(1.0f, xDistance / static_cast<float>(HALF_WIDTH));
    const float weight2 = 1.0f - weight1;
    const float targetY = weight1 * targetY1 + weight2 * targetY2;

    const float maxSpeed = static_cast<float>(MAX_AI_SPEED);
    return std::min(maxSpeed, std::max(-maxSpeed, targetY - y));
}

float P1Controls()
{
    if (IsKeyDown(KEY_Z) || IsKeyDown(KEY_DOWN)) {
        return static_cast<float>(PLAYER_SPEED);
    }
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_UP)) {
        return -static_cast<float>(PLAYER_SPEED);
    }
    return 0.0f;
}

float P2Controls()
{
    if (IsKeyDown(KEY_M)) {
        return static_cast<float>(PLAYER_SPEED);
    }
    if (IsKeyDown(KEY_K)) {
        return -static_cast<float>(PLAYER_SPEED);
    }
    return 0.0f;
}

} // namespace boing