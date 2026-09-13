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
    class TileLayer final : public component::Component {
    public:
        TileLayer(const entt::hashed_string& name);
        ~TileLayer()override;
    public:
        void Set_VisibleState(bool visibleState);
        bool Get_VisibleState() const { return this->visibleState_; }
    protected:
        void Init() override;
        void Update(float, core::Context&) override;
        void Render(core::Context& context) override;
        void Clean() override;
    public:
        entt::hashed_string name_;
        glm::vec2 tileSize_ = glm::vec2(0.f);
        glm::vec2 mapSize_ = glm::vec2(0.f);
        TransformComponent* transformComponent_ = nullptr;
        SDL_Texture* texture_ = nullptr;
        bool visibleState_ = false;
    private:
        const char* Get_ClassName() const {
            return typeid(*this).name();
        }
    };
} // namespace engine::component
