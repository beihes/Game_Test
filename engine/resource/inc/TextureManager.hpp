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

#include <raii/sdlRaii.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>

namespace engine::resource {
    class TextureManager final {
    public:
        TextureManager(SDL_Renderer* renderer);
        ~TextureManager();
    public:
        SDL_Texture* Insert(std::string_view texturePath);
        SDL_Texture* Insert(std::string_view midName, SDL_Surface* midSurface_Ptr);
        /* 这个方式会把指针占为己有，不允许外部释放 */
        SDL_Texture* Insert(std::string_view midName, SDL_Texture* midTexture);
        /* 此方式会将传入的智能指针据为己有， */
        SDL_Texture* Insert(std::string_view midName, raii::SDL_TexturePtr&& midTexturePtr);

        SDL_Texture* Search(std::string_view midName);
        bool Delete(std::string_view midName);
    public:
        void Clean();
    private:
        std::unordered_map<std::string, raii::SDL_TexturePtr> textures_;
        SDL_Renderer* renderer_;
    private:
        const char* Get_ClassName() const {
            return typeid(*this).name();
        }
    };
}