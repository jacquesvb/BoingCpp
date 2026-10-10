#include "game.h"

#include <algorithm>
#include <cstdio>
#include <string>

#include "assets.h"
#include "audio.h"
#include "constants.h"

namespace boing {

namespace {

// Formats a score as (at least) two digits, e.g. 0 -> "00", 10 -> "10".
std::string FormatScore(int score)
{
    char buffer[8];
    std::snprintf(buffer, sizeof(buffer), "%02d", score);
    return std::string(buffer);
}

} // namespace

Game::Game(Audio& audio, ControlFunc p1, ControlFunc p2)
    : audio_(audio),
      bats_{Bat(0, std::move(p1)), Bat(1, std::move(p2))},
      ball_(-1.0f)
{
}

void Game::Update()
{
    // Update the bats and the ball. The set of impacts to update is the one
    // that existed before the ball moved, so an impact created by this frame's
    // bounce is not advanced until the next frame (matching the original).
    for (Bat& bat : bats_) {
        bat.Update(*this);
    }

    const std::size_t impactCount = impacts_.size();
    ball_.Update(*this);

    for (std::size_t i = 0; i < impactCount; ++i) {
        impacts_[i].Update();
    }

    // Remove impacts that have finished playing, iterating backwards so the
    // remaining indices stay valid.
    for (int i = static_cast<int>(impacts_.size()) - 1; i >= 0; --i) {
        if (impacts_[i].Time() >= 10) {
            impacts_.erase(impacts_.begin() + i);
        }
    }

    if (ball_.Out()) {
        const int scoringPlayer = (ball_.x < WIDTH / 2) ? 1 : 0;
        const int losingPlayer = 1 - scoringPlayer;

        // The conceding player's timer controls both the animation and the
        // delay before a new ball appears.
        if (bats_[losingPlayer].Timer() < 0) {
            bats_[scoringPlayer].AddScore();
            PlaySound("score_goal", 1);
            bats_[losingPlayer].SetTimer(BAT_SCORE_TIMER);
        } else if (bats_[losingPlayer].Timer() == 0) {
            const float direction = (losingPlayer == 0) ? -1.0f : 1.0f;
            ball_ = Ball(direction);
        }
    }
}

void Game::Draw(Assets& assets) const
{
    assets.Blit("table", 0.0f, 0.0f);

    // 'Just scored' overlay effects.
    for (int p = 0; p < 2; ++p) {
        if (bats_[p].Timer() > 0 && ball_.Out()) {
            assets.Blit("effect" + std::to_string(p), 0.0f, 0.0f);
        }
    }

    for (const Bat& bat : bats_) {
        bat.Draw(assets);
    }
    ball_.Draw(assets);
    for (const Impact& impact : impacts_) {
        impact.Draw(assets);
    }

    // Scores, drawn as two digit sprites per player.
    for (int p = 0; p < 2; ++p) {
        const std::string score = FormatScore(bats_[p].Score());
        const int otherPlayer = 1 - p;

        for (int i = 0; i < 2; ++i) {
            char colour = '0';
            if (bats_[otherPlayer].Timer() > 0 && ball_.Out()) {
                colour = (p == 0) ? '2' : '1';
            }

            const std::string image = std::string("digit") + colour + score[static_cast<std::size_t>(i)];
            assets.Blit(image, 255.0f + (160.0f * p) + (i * 55.0f), 46.0f);
        }
    }
}

void Game::PlaySound(const std::string& name, int count, bool menuSound)
{
    // In-game sound effects are muted while player 0 is computer controlled
    // (i.e. while the game is in attract mode on the menu).
    if (!bats_[0].IsHuman() && !menuSound) {
        return;
    }
    audio_.Play(name, count);
}

} // namespace boing