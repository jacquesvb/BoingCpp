#pragma once

#include <memory>

#include "assets.h"
#include "audio.h"
#include "game.h"

namespace boing {

enum class State {
    Menu,
    Play,
    GameOver,
};

// Owns the window, the game loop and the top level state machine
// (menu / playing / game over), including attract mode.
class Application {
public:
    Application() = default;

    // Runs the game until the window is closed. Returns the process exit code.
    int Run();

private:
    void Update();
    void Draw();

    // Makes sure relative "resources/..." paths resolve when the binary is
    // started from a different working directory.
    void ResolveResourceDirectory() const;

    State state_ = State::Menu;
    int numPlayers_ = 1;
    bool spaceDown_ = false;

    std::unique_ptr<Game> game_;
    Assets assets_;
    Audio audio_;
};

} // namespace boing