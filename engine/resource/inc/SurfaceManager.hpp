#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
    //#include <SDL3_ttf/SDL_ttf.h>
#ifdef __cplusplus
}
#endif // __cplusplus

#include "sdl.hpp"
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace engine::resource {
    class SurfaceManager final {
    public:
        SurfaceManager();
        ~SurfaceManager();
    public:
        SDL_Surface* Insert(std::string_view surfacePath);
        /* 这个会将传入的 Surface 复制一份 */
        SDL_Surface* Insert(std::string_view midName, SDL_Surface* midSurface);
        /* 此方式会将传入的智能指针据为己有 */
        SDL_Surface* Insert(std::string_view midName, sdl::SDL_SurfacePtr&& midSurfacePtr);
        SDL_Surface* Search(std::string_view midName);
        bool Delete(std::string_view midName);
    public:
        void Clean();
    private:
        std::unordered_map<std::string, sdl::SDL_SurfacePtr> surfaces_;
    private:
        const char* Get_ClassName() const {
            return typeid(*this).name();
        }
    };
}