#pragma once

#include <string>
#include <unordered_map>

#include "raylib.h"

namespace boing {

// Owns and caches all textures loaded from the images resource folder.
// Mirrors the way Pygame Zero lazily loads images by name and blits them
// at their native size.
class Assets {
public:
    // Verifies the images folder can be found from the current directory.
    bool Init() const;

    // Unloads every cached texture.
    void Shutdown();

    // Returns the cached texture for the given image name (e.g. "ball"),
    // loading it from disk on first use. A zero-sized texture is returned
    // when the file cannot be found (nothing is drawn for it).
    Texture2D& Get(const std::string& name);

    // Draws an image with its top-left corner at (x, y).
    void Blit(const std::string& name, float x, float y);

    // Draws an image centred on (x, y).
    void BlitCentered(const std::string& name, float x, float y);

private:
    std::string EnsureSuffix(const std::string& name) const;
    std::string Path(const std::string& name) const;

    std::unordered_map<std::string, Texture2D> textures_;
    Texture2D empty_{0, 0, 0, 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8};
};

} // namespace boing