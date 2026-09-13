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

#include "engine/component/inc/Component.hpp"
#include "entt/entt.hpp"
#include <glm/vec2.hpp>

namespace engine::component::tiled {
    class Tile final : public component::Component {
    public:
        Tile(/* args */);
        ~Tile();
    protected:
        void Init() override;
        void Update(float, core::Context&) override;
        void Render(core::Context& context) override;
        void Clean() override;
    public:
        bool visibleState_ = false;
        int gid = 0;
        int x = 0;
        int y = 0;
    private:
        const char* Get_ClassName() const {
            return typeid(*this).name();
        }
    };



} // namespace engine::component::tiled
