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

namespace app::scene {
    class AppContext;
} // namespace app::scene

namespace engine::component::tiled {
    class TileLayer;
} // namespace engine::component::tiled

namespace engine::render {
    class Sprite;
} // namespace engine::render


namespace app::tiled {
    class Map {
    public:
        Map();
        ~Map();
    public:
        void Init(const std::string& directory, const std::string& name, scene::AppContext& appContext);
        void Input(const SDL_Event& event);
        void Update(float deltaTime);
        void Render();
        void Clear();
    public:
        std::unique_ptr<::tiled::Map> map_;
    private:
        std::optional<std::reference_wrapper<scene::AppContext>> appContext_;
    private:
        const char* Get_ClassName() const {
            return typeid(*this).name();
        }
    };



} // namespace app::tiled
