#include "application.h"

#include <algorithm>
#include <string>

#include "constants.h"

namespace boing {

namespace {

// Image used to check whether the resources folder is reachable.
constexpr const char* kResourceProbe = "resources/images/table.png";

} // namespace

void Application::ResolveResourceDirectory() const
{
    if (FileExists(kResourceProbe)) {
        return;
    }

    const std::string appDirectory = GetApplicationDirectory();
    const std::string probe = appDirectory + kResourceProbe;
    if (FileExists(probe.c_str())) {
        ChangeDirectory(appDirectory.c_str());
    }
}

int Application::Run()
{
    SetTraceLogLevel(LOG_WARNING);
    InitWindow(WIDTH, HEIGHT, TITLE);
    if (!IsWindowReady()) {
        TraceLog(LOG_FATAL, "Failed to create the game window.");
        return 1;
    }
    SetTargetFPS(TARGET_FPS);

    ResolveResourceDirectory();

    InitAudioDevice();
    audio_.Init();
    assets_.Init();
    audio_.PlayMusic();

    // Start in attract mode: two computer players on the menu background.
    game_ = std::make_unique<Game>(audio_);

    while (!WindowShouldClose()) {
        Update();
        audio_.Update();

        BeginDrawing();
        ClearBackground(BLACK);
        Draw();
        EndDrawing();
    }

    game_.reset();
    assets_.Shutdown();
    audio_.Shutdown();
    CloseAudioDevice();
    CloseWindow();
    return 0;
}

void Application::Update()
{
    // Work out whether the space key has just been pressed this frame.
    const bool spaceDown = IsKeyDown(KEY_SPACE);
    const bool spacePressed = spaceDown && !spaceDown_;
    spaceDown_ = spaceDown;

    switch (state_) {
    case State::Menu:
        if (spacePressed) {
            state_ = State::Play;
            if (numPlayers_ == 2) {
                game_ = std::make_unique<Game>(audio_, P1Controls, P2Controls);
            } else {
                game_ = std::make_unique<Game>(audio_, P1Controls);
            }
        } else {
            // Switch between one and two players.
            if (numPlayers_ == 2 && IsKeyDown(KEY_UP)) {
                game_->PlaySound("up", 1, true);
                numPlayers_ = 1;
            } else if (numPlayers_ == 1 && IsKeyDown(KEY_DOWN)) {
                game_->PlaySound("down", 1, true);
                numPlayers_ = 2;
            }

            // Keep the attract mode game running in the background.
            game_->Update();
        }
        break;

    case State::Play:
        if (std::max(game_->GetBat(0).Score(), game_->GetBat(1).Score()) > WINNING_SCORE) {
            state_ = State::GameOver;
        } else {
            game_->Update();
        }
        break;

    case State::GameOver:
        if (spacePressed) {
            state_ = State::Menu;
            numPlayers_ = 1;
            game_ = std::make_unique<Game>(audio_);
        }
        break;
    }
}

void Application::Draw()
{
    game_->Draw(assets_);

    if (state_ == State::Menu) {
        assets_.Blit("menu" + std::to_string(numPlayers_ - 1), 0.0f, 0.0f);
    } else if (state_ == State::GameOver) {
        assets_.Blit("over", 0.0f, 0.0f);
    }
}

} // namespace boing