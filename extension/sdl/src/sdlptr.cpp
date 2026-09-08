#include "sdlptr.hpp"

namespace sdl {

    void VoidDelete::operator()(void* midVoid) const {
        if (!midVoid) { return; }
        SDL_free(midVoid);
    }

    // ==================== Window ====================

    void SDL_WindowDelete::operator()(SDL_Window* midWindow) const {
        if (!midWindow) { return; }
        SDL_DestroyWindow(midWindow);
    }

    // ==================== Tray ====================

    void SDL_TrayDelete::operator()(SDL_Tray* midTray) const {
        if (!midTray) { return; }
        SDL_DestroyTray(midTray);
    }

    // ==================== Camera ====================

    void SDL_CameraIDDelete::operator()(SDL_CameraID* midCameraIDPtr) const {
        if (!midCameraIDPtr) { return; }
        SDL_free(midCameraIDPtr);
    }

    void SDL_CameraSpecDelete::operator()(SDL_CameraSpec** midCameraSpec2Ptr) const {
        if (!midCameraSpec2Ptr) { return; }
        SDL_free(midCameraSpec2Ptr);
    }

    void SDL_CameraDelete::operator()(SDL_Camera* midCamera) const {
        if (!midCamera) { return; }
        SDL_CloseCamera(midCamera);
    }

    // ==================== OpenGL ====================

    void SDL_GLContextDelete::operator()(std::remove_pointer_t<SDL_GLContext>* midGLContext) const {
        if (!midGLContext) { return; }
        SDL_GL_DestroyContext(midGLContext);
    }

    // ==================== Renderer ====================

    void SDL_RendererDelete::operator()(SDL_Renderer* midRenderer) const {
        if (!midRenderer) { return; }
        SDL_DestroyRenderer(midRenderer);
    }

    void SDL_GPUDeviceDelete::operator()(SDL_GPUDevice* midDevice) const {
        if (!midDevice) { return; }
        SDL_DestroyGPUDevice(midDevice);
    }

    // ==================== Texture ====================

    void SDL_TextureDelete::operator()(SDL_Texture* midTexture) const {
        if (!midTexture) { return; }
        SDL_DestroyTexture(midTexture);
    }

    void SDL_GPUTextureDelete::operator()(std::pair<SDL_GPUDevice*, SDL_GPUTexture*>* midPair) const {
        if (!midPair) { return; }

        if (midPair->first && midPair->second) {
            SDL_ReleaseGPUTexture(midPair->first, midPair->second);
        }

        delete midPair;
    }

    // ==================== Surface ====================

    void SDL_SurfaceDelete::operator()(SDL_Surface* midSurface) const {
        if (!midSurface) { return; }
        SDL_DestroySurface(midSurface);
    }

    // ==================== Palette ====================

    void SDL_PaletteDelete::operator()(SDL_Palette* midPalette) const {
        if (!midPalette) { return; }
        SDL_DestroyPalette(midPalette);
    }

    // ==================== Cursor ====================

    void SDL_CursorDelete::operator()(SDL_Cursor* midCursor) const {
        if (!midCursor) { return; }
        SDL_DestroyCursor(midCursor);
    }

    // ==================== Audio ====================

    void SDL_AudioStreamDelete::operator()(SDL_AudioStream* midAudioStream) const {
        if (!midAudioStream) { return; }
        SDL_DestroyAudioStream(midAudioStream);
    }

#ifdef SDL_MIXER_H_

#endif

    // ==================== SDL_ttf ====================
#ifdef SDL_TTF_H_
    void TTF_FontDelete::operator()(TTF_Font* midFont) const {
        if (!midFont) { return; }
        TTF_CloseFont(midFont);
    }
#endif
}