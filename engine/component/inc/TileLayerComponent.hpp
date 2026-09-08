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

#include "Component.hpp"
#include <glm/vec2.hpp>

#include <cstdint>

namespace engine::core {
    class Context;
}

namespace tiled {
    class TileLayer;
    class TileSet;
}

namespace engine::component {
    class TransformComponent;
    class TileLayerComponent final : public component::Component {
        friend class object::Object;//友元不能继承，必须每个子类单独添加
    public:
        TileLayerComponent();
        ~TileLayerComponent()override;
    public:
        void Set_Layer(const tiled::TileLayer* layer);
        void Set_TileSet(const tiled::TileSet* tileSet);
        void Set_Texture(SDL_Texture* texture);
        void Set_Visible(bool visible);
        bool Is_Visible() const { return this->visible_; }
    protected:
        void Init() override;
        void Update(float, core::Context&) override;
        void Render(core::Context& context) override;
        void Clean() override;
    private:
        glm::vec2 tileSize_ = glm::vec2(0.f);
        glm::vec2 mapSize_ = glm::vec2(0.f);
        TransformComponent* transformComponent_ = nullptr;
        const tiled::TileLayer* layer_ = nullptr;
        const tiled::TileSet* tileSet_ = nullptr;
        SDL_Texture* texture_ = nullptr;
        bool enabled_ = true;
        bool visible_ = false;
    private:
        const char* Get_ClassName() const {
            return typeid(*this).name();
        }
    };
} // namespace engine::component
