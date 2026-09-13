#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#ifdef __cplusplus
}
#endif // __cplusplus

#include <tiled/tiled.hpp>
#include <entt/entt.hpp>
#include <memory>
#include <optional>
#include <typeinfo>

namespace engine::component::tiled {
    class Layer;
} // namespace engine::conponent::tiled


namespace app::tiled::tilelayer {
    void Render_Callback(engine::component::tiled::Layer& tileLayer, void* userData);
} // namespace app::tiled
