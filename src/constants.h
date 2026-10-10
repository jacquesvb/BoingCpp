#pragma once

namespace boing {

// Window / level dimensions.
constexpr int WIDTH = 800;
constexpr int HEIGHT = 480;
constexpr const char* TITLE = "Boing!";

constexpr int HALF_WIDTH = WIDTH / 2;
constexpr int HALF_HEIGHT = HEIGHT / 2;

// Movement speeds.
constexpr int PLAYER_SPEED = 6;
constexpr int MAX_AI_SPEED = 6;

// Boundaries the bats are clamped to on the Y axis.
constexpr float BAT_MIN_Y = 80.0f;
constexpr float BAT_MAX_Y = 400.0f;

// Collision thresholds (see the comments in the original Python source).
constexpr float BALL_BAT_X_THRESHOLD = 344.0f;
constexpr float BALL_WALL_Y_DISTANCE = 220.0f;
constexpr float BAT_COLLISION_HALF_RANGE = 64.0f;
constexpr float BALL_DEFLECT_DIVISOR = 128.0f;

constexpr int BAT_HIGHLIGHT_TIMER = 10;
constexpr int BAT_SCORE_TIMER = 20;
constexpr int WINNING_SCORE = 9;

// Pygame Zero tick rate the game was tuned to.
constexpr int TARGET_FPS = 60;

// Where assets live relative to the working directory.
constexpr const char* IMAGE_DIR = "resources/images/";
constexpr const char* SOUND_DIR = "resources/sounds/";
constexpr const char* MUSIC_DIR = "resources/music/";

} // namespace boing