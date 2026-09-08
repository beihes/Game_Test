#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include <SDL3/SDL.h>
    //#include <SDL3_image/SDL_image.h>
    //#include <SDL3_ttf/SDL_ttf.h>
#ifdef __cplusplus
}
#endif // __cplusplus

#include <memory>
#include <optional>

namespace engine::core {
    class Context;
}

namespace engine::resource {
    class SurfaceManager;
    class TextureManager;
    class ResourceManager final {
    public:
        ResourceManager();
        ~ResourceManager();
    public:
        bool Init(core::Context& context);
        void Clean();
    public:
        SurfaceManager* Get_SurfaceManager();
        TextureManager* Get_TextureManager();
    private:
        std::unique_ptr<SurfaceManager> surfaceManager_;
        std::unique_ptr<TextureManager> textureManager_;
    private:
        std::optional<std::reference_wrapper<core::Context>> context_;
    private:
        const char* Get_ClassName() const {
            return typeid(*this).name();
        }
    };
}