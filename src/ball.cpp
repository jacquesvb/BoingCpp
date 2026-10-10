#include "ball.h"

#include <algorithm>
#include <cmath>

#include "bat.h"
#include "game.h"
#include "math_utils.h"

namespace boing {

Ball::Ball(float dx)
    : Actor("ball", static_cast<float>(HALF_WIDTH), static_cast<float>(HALF_HEIGHT)),
      dx_(dx),
      dy_(0.0f)
{
}

void Ball::Update(Game& game)
{
    // Move in a series of small steps; the number of steps is based on speed.
    for (int step = 0; step < speed_; ++step) {
        const float originalX = x;

        x += dx_;
        y += dy_;

        // Check whether the ball's edge has just crossed the plane of a bat.
        if (std::fabs(x - HALF_WIDTH) >= BALL_BAT_X_THRESHOLD &&
            std::fabs(originalX - HALF_WIDTH) < BALL_BAT_X_THRESHOLD) {

            float newDirX = 1.0f;
            int batIndex = 0;
            if (x >= HALF_WIDTH) {
                newDirX = -1.0f;
                batIndex = 1;
            }

            Bat& bat = game.GetBat(batIndex);
            const float differenceY = y - bat.y;

            if (differenceY > -BAT_COLLISION_HALF_RANGE &&
                differenceY < BAT_COLLISION_HALF_RANGE) {

                // Bounce back on the X axis, then deflect up or down
                // depending on where the ball hit the bat.
                dx_ = -dx_;
                dy_ += differenceY / BALL_DEFLECT_DIVISOR;
                dy_ = std::min(1.0f, std::max(-1.0f, dy_));

                const Vector2 direction = Normalized(Vector2{dx_, dy_});
                dx_ = direction.x;
                dy_ = direction.y;

                game.AddImpact(x - newDirX * 10.0f, y);
                ++speed_;
                game.SetAiOffset(GetRandomValue(-10, 10));
                bat.SetTimer(BAT_HIGHLIGHT_TIMER);

                game.PlaySound("hit", 5);
                if (speed_ <= 10) {
                    game.PlaySound("hit_slow", 1);
                } else if (speed_ <= 12) {
                    game.PlaySound("hit_medium", 1);
                } else if (speed_ <= 16) {
                    game.PlaySound("hit_fast", 1);
                } else {
                    game.PlaySound("hit_veryfast", 1);
                }
            }
        }

        // Bounce off the top and bottom of the arena.
        if (std::fabs(y - HALF_HEIGHT) > BALL_WALL_Y_DISTANCE) {
            dy_ = -dy_;
            y += dy_;

            game.AddImpact(x, y);

            game.PlaySound("bounce", 5);
            game.PlaySound("bounce_synth", 1);
        }
    }
}

} // namespace boing