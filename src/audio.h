#pragma once

#include <string>
#include <unordered_map>

#include "raylib.h"

namespace boing {

// Preloads and plays the game's sound effects and streams the theme music.
//
// Sound effects follow the same naming convention as the Python original
// (e.g. "hit0.ogg" ... "hit4.ogg"): Play() picks one of the variants at
// random. When a "<name><index>.ogg" file does not exist, a plain
// "<name>.ogg" is used as a fallback, which lets the menu "up"/"down" assets
// be heard even though the original code only looked for numbered variants.
class Audio {
public:
    // Loads all sounds and the music stream. Returns false if no audio
    // device is available (the game keeps running silently).
    bool Init();

    void Shutdown();

    // Must be called every frame so the music stream keeps playing.
    void Update();

    void PlayMusic() const;

    // Plays a random variant named "<base><index>.ogg" for index in
    // [0, count). Falls back to "<base>.ogg" when the variant is missing.
    void Play(const std::string& base, int count = 1);

    bool IsReady() const { return ready_; }

private:
    Sound* FindSound(const std::string& key);

    std::unordered_map<std::string, Sound> sounds_;
    Music music_{};
    bool ready_ = false;
    bool musicLoaded_ = false;
};

} // namespace boing