#pragma once

#ifndef sdlptr_h_
#define sdlptr_h_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include <SDL3/SDL.h>
// #include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>
#ifdef __cplusplus
}
#endif // __cplusplus

#include <memory>
#include <unordered_map>

namespace sdl {

    struct VoidDelete {
        void operator()(void* midVoid) const;
    };

    using VoidPtr = std::unique_ptr<void, VoidDelete>;

    // ==================== Window ====================

    struct SDL_WindowDelete {
        void operator()(SDL_Window* midWindow) const;
    };

    using SDL_WindowPtr = std::unique_ptr<SDL_Window, SDL_WindowDelete>;

    // ==================== Tray ====================

    struct SDL_TrayDelete {
        void operator()(SDL_Tray* midTray) const;
    };

    using SDL_TrayPtr = std::unique_ptr<SDL_Tray, SDL_TrayDelete>;

    struct SDL_TrayType {
        SDL_TrayPtr tray;
        SDL_TrayMenu* menu = nullptr;
        std::unordered_map<SDL_TrayEntry*, std::pair<SDL_TrayCallback, void*>> entry;
    };

    // ==================== Camera ====================

    struct SDL_CameraIDDelete {
        void operator()(SDL_CameraID* midCameraIDPtr) const;
    };

    using SDL_CameraIDPtr = std::unique_ptr<SDL_CameraID, SDL_CameraIDDelete>;

    struct SDL_CameraSpecDelete {
        void operator()(SDL_CameraSpec** midCameraSpec2Ptr) const;
    };

    using SDL_CameraSpecPtrPtr = std::unique_ptr<SDL_CameraSpec*, SDL_CameraSpecDelete>;

    struct SDL_CameraDelete {
        void operator()(SDL_Camera* midCamera) const;
    };

    using SDL_CameraPtr = std::unique_ptr<SDL_Camera, SDL_CameraDelete>;

    // ==================== OpenGL ====================

    struct SDL_GLContextDelete {
        void operator()(std::remove_pointer_t<SDL_GLContext>* midGLContext) const;
    };

    using SDL_GLContextPtr = std::unique_ptr<std::remove_pointer_t<SDL_GLContext>, SDL_GLContextDelete>;

    // ==================== Renderer ====================

    struct SDL_RendererDelete {
        void operator()(SDL_Renderer* midRenderer) const;
    };

    using SDL_RendererPtr = std::unique_ptr<SDL_Renderer, SDL_RendererDelete>;

    struct SDL_GPUDeviceDelete {
        void operator()(SDL_GPUDevice* midDevice) const;
    };

    using SDL_GPUDevicePtr = std::unique_ptr<SDL_GPUDevice, SDL_GPUDeviceDelete>;

    // ==================== Texture ====================

    struct SDL_TextureDelete {
        void operator()(SDL_Texture* midTexture) const;
    };

    using SDL_TexturePtr = std::unique_ptr<SDL_Texture, SDL_TextureDelete>;

    struct SDL_GPUTextureDelete {
        void operator()(std::pair<SDL_GPUDevice*, SDL_GPUTexture*>* midPair) const;
    };

    using SDL_GPUTexturePtr = std::unique_ptr<std::pair<SDL_GPUDevice*, SDL_GPUTexture*>, SDL_GPUTextureDelete>;

    // ==================== Surface ====================

    struct SDL_SurfaceDelete {
        void operator()(SDL_Surface* midSurface) const;
    };

    using SDL_SurfacePtr = std::unique_ptr<SDL_Surface, SDL_SurfaceDelete>;

    // ==================== Palette ====================

    struct SDL_PaletteDelete {
        void operator()(SDL_Palette* midPalette) const;
    };

    using SDL_PalettePtr = std::unique_ptr<SDL_Palette, SDL_PaletteDelete>;

    // ==================== Cursor ====================

    struct SDL_CursorDelete {
        void operator()(SDL_Cursor* midCursor) const;
    };

    using SDL_CursorPtr = std::unique_ptr<SDL_Cursor, SDL_CursorDelete>;

    // ==================== Audio ====================

    struct SDL_AudioStreamDelete {
        void operator()(SDL_AudioStream* midAudioStream) const;
    };

    using SDL_AudioStreamPtr = std::unique_ptr<SDL_AudioStream, SDL_AudioStreamDelete>;

#ifdef SDL_MIXER_H_

#endif

#ifdef SDL_TTF_H_
    struct TTF_FontDelete {
        void operator()(TTF_Font* midFont) const;
    };

    using TTF_FontPtr = std::unique_ptr<TTF_Font, TTF_FontDelete>;
#endif

}

#endif