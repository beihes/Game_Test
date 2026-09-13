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
#include <raii/sdlRaii.hpp>

namespace engine::component {
    class TransformComponent;
} // namespace engine::component

namespace engine::core {
    class Context;
}

namespace tiled {
    class TileLayer;
    class TileSet;
}

namespace engine::component::tiled {
    struct TileData {
        uint32_t gid = 0;      // 0 表示空瓦片
        glm::ivec2 position;
    };
    class Map final : public component::Component {
    public:
        Map(const entt::hashed_string& name);
        ~Map()override;
    protected:
        void Init() override;
        void Update(float, core::Context&) override;
        void Render(core::Context& context) override;
        void Clean() override;
    public:
        void Set_VisibleState(bool visibleState) { this->visibleState_ = visibleState; }
        bool Get_VisibleState() const { return this->visibleState_; }
    public:
        entt::hashed_string name_;
        bool visibleState_;
        int compressionLevel_ = 0;
        int height_ = 0;
        int width_ = 0;
        int tileHeight_ = 0;
        int tileWidth_ = 0;
        int nextLayerId_ = 0;
        int nextObjectId_ = 0;
        bool infinite_ = false;
        raii::SDL_TexturePtr texture_;
    private:
        const char* Get_ClassName() const {
            return typeid(*this).name();
        }
    };
} // namespace engine::component
