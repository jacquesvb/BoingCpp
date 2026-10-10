#include "assets.h"

#include <string>

#include "constants.h"

namespace boing {

bool Assets::Init() const
{
    if (FileExists(Path("table").c_str())) {
        return true;
    }

    TraceLog(LOG_ERROR, "Could not find the images folder ('%s').", IMAGE_DIR);
    return false;
}

void Assets::Shutdown()
{
    for (const auto& entry : textures_) {
        if (entry.second.id != 0) {
            UnloadTexture(entry.second);
        }
    }
    textures_.clear();
}

std::string Assets::EnsureSuffix(const std::string& name) const
{
    if (name.size() >= 4 && name.compare(name.size() - 4, 4, ".png") == 0) {
        return name;
    }
    return name + ".png";
}

std::string Assets::Path(const std::string& name) const
{
    return std::string(IMAGE_DIR) + EnsureSuffix(name);
}

Texture2D& Assets::Get(const std::string& name)
{
    auto it = textures_.find(name);
    if (it != textures_.end()) {
        return it->second;
    }

    const std::string path = Path(name);
    if (!FileExists(path.c_str())) {
        TraceLog(LOG_WARNING, "Image not found: '%s'", path.c_str());
        return empty_;
    }

    Texture2D texture = LoadTexture(path.c_str());
    if (texture.id == 0) {
        TraceLog(LOG_WARNING, "Failed to load image: '%s'", path.c_str());
        return empty_;
    }
    textures_.emplace(name, texture);
    return textures_.at(name);
}

void Assets::Blit(const std::string& name, float x, float y)
{
    const Texture2D& texture = Get(name);
    if (texture.width == 0 || texture.height == 0) {
        return;
    }
    DrawTexturePro(
        texture,
        Rectangle{0.0f, 0.0f, static_cast<float>(texture.width), static_cast<float>(texture.height)},
        Rectangle{x, y, static_cast<float>(texture.width), static_cast<float>(texture.height)},
        Vector2{0.0f, 0.0f},
        0.0f,
        WHITE);
}

void Assets::BlitCentered(const std::string& name, float x, float y)
{
    const Texture2D& texture = Get(name);
    if (texture.width == 0 || texture.height == 0) {
        return;
    }
    DrawTexturePro(
        texture,
        Rectangle{0.0f, 0.0f, static_cast<float>(texture.width), static_cast<float>(texture.height)},
        Rectangle{x - texture.width * 0.5f, y - texture.height * 0.5f,
                  static_cast<float>(texture.width), static_cast<float>(texture.height)},
        Vector2{0.0f, 0.0f},
        0.0f,
        WHITE);
}

} // namespace boing