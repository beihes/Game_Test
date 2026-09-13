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
#include "engine/render/inc/Sprite.hpp"
#include "entt/entt.hpp"
#include <glm/vec2.hpp>

namespace engine::component {
    class TransformComponent;
} // namespace engine::component

namespace engine::core {
    class Context;
}

namespace engine::component::tiled {
    class Layer final : public component::Component {
    public:
        Layer(uint32_t id, const glm::ivec2& mapSize, const glm::ivec2& tileSize, std::vector<render::Sprite>& spriteVector, void* userData = nullptr);
        ~Layer()override;
    public:
        void Set_VisibleState(bool visibleState);
        bool Get_VisibleState() const { return this->visibleState_; }
    public:
        void Init() override;
        void Update(float, core::Context&) override;
        void Render(core::Context& context) override;
        void Clean() override;
    public:
        uint32_t id_ = 0;
        glm::ivec2 mapSize_ = glm::ivec2(0);
        glm::ivec2 tileSize_ = glm::ivec2(0);
        TransformComponent* transformComponent_ = nullptr;
        std::vector<render::Sprite> spriteVector_;
        SDL_Texture* texture_ = nullptr;
        bool visibleState_ = false;
    public:
        using RenderCallback = void(*)(Layer&, void*);
        RenderCallback renderCallback = nullptr;
    private:
        void* userData_ = nullptr;
    private:
        const char* Get_ClassName() const {
            return typeid(*this).name();
        }
    };
} // namespace engine::component
