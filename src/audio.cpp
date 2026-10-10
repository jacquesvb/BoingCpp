#include "audio.h"

#include <string>
#include <vector>

#include "constants.h"

namespace boing {

namespace {

// Every sound effect referenced by the game (file name without extension).
const std::vector<std::string> kSoundNames = {
    "bounce0", "bounce1", "bounce2", "bounce3", "bounce4",
    "bounce_synth0",
    "hit0", "hit1", "hit2", "hit3", "hit4",
    "hit_slow0",
    "hit_medium0",
    "hit_fast0",
    "hit_veryfast0",
    "score_goal0",
    "up",
    "down",
};

} // namespace

bool Audio::Init()
{
    if (!IsAudioDeviceReady()) {
        TraceLog(LOG_WARNING, "Audio device not available; running without sound.");
        return false;
    }

    for (const std::string& name : kSoundNames) {
        const std::string path = std::string(SOUND_DIR) + name + ".ogg";
        if (!FileExists(path.c_str())) {
            TraceLog(LOG_WARNING, "Sound not found, skipping: '%s'", path.c_str());
            continue;
        }

    Sound sound = LoadSound(path.c_str());
    if (sound.frameCount == 0 || sound.stream.buffer == nullptr) {
        TraceLog(LOG_WARNING, "Failed to load sound: '%s'", path.c_str());
        continue;
    }
    sounds_.emplace(name, sound);
    }

    const std::string musicPath = std::string(MUSIC_DIR) + "theme.ogg";
    if (FileExists(musicPath.c_str())) {
        music_ = LoadMusicStream(musicPath.c_str());
        musicLoaded_ = music_.frameCount > 0;
        if (musicLoaded_) {
            SetMusicVolume(music_, 0.3f);
        } else {
            TraceLog(LOG_WARNING, "Failed to load music: '%s'", musicPath.c_str());
        }
    }

    ready_ = true;
    return true;
}

void Audio::Shutdown()
{
    if (musicLoaded_) {
        StopMusicStream(music_);
        UnloadMusicStream(music_);
        musicLoaded_ = false;
    }

    for (const auto& entry : sounds_) {
        if (entry.second.stream.buffer != nullptr) {
            UnloadSound(entry.second);
        }
    }
    sounds_.clear();
    ready_ = false;
}

void Audio::Update()
{
    if (ready_ && musicLoaded_) {
        UpdateMusicStream(music_);
    }
}

void Audio::PlayMusic() const
{
    if (ready_ && musicLoaded_) {
        PlayMusicStream(music_);
    }
}

Sound* Audio::FindSound(const std::string& key)
{
    auto it = sounds_.find(key);
    return it == sounds_.end() ? nullptr : &it->second;
}

void Audio::Play(const std::string& base, int count)
{
    if (!ready_ || count < 1) {
        return;
    }

    const int index = (count > 1) ? GetRandomValue(0, count - 1) : 0;
    const std::string key = base + std::to_string(index);

    const Sound* sound = FindSound(key);
    if (sound == nullptr) {
        sound = FindSound(base);
    }
    if (sound != nullptr) {
        PlaySound(*sound);
    }
}

} // namespace boing