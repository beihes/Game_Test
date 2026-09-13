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

#include <raii/sdlRaii.hpp>
#include <string>
#include <typeinfo>

namespace engine::input {
    class InputManager;
}

namespace engine::resource {
    class ResourceManager;
}

namespace engine::scene {
    class SceneManager;
}

namespace engine::core {
    class Config;
    class Context {
    public:
        Context(Config& config, input::InputManager& inputManager, resource::ResourceManager& resourceManager, SDL_Window* window, raii::SDL_TrayType& trayType, SDL_Renderer* renderer, bool& runningState, std::atomic<SDL_AppResult>& appResult);
        ~Context();
    public:
        Config& Get_Config()const;
        input::InputManager& Get_InputManager()const;
        resource::ResourceManager& Get_ResourceManager()const;
        SDL_Window* Get_Window()const;
        SDL_Renderer* Get_Renderer()const;
        raii::SDL_TrayType& Get_TrayType()const { return this->trayType_; }
        bool& Get_RunningState()const;
        std::atomic<SDL_AppResult>& Get_AppResult()const;
    private:
        Config& config_;
        input::InputManager& inputManager_;
        resource::ResourceManager& resourceManager_;
        SDL_Window* window_;
        SDL_Renderer* renderer_;
        raii::SDL_TrayType& trayType_;
        bool& runningState_;
        std::atomic<SDL_AppResult>& appResult_;
    private:
        const char* Get_ClassName() const {
            return typeid(*this).name();
        }
    };
}