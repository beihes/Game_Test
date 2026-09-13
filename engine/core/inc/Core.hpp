#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include <SDL3/SDL.h>
    // #include <SDL3/SDL_vulkan.h>
    //#include <SDL3_image/SDL_image.h>
    //#include <SDL3_ttf/SDL_ttf.h>
#ifdef __cplusplus
}
#endif // __cplusplus

#include <datetime/datetime.hpp>
#include <raii/sdlRaii.hpp>
#include <imgui.h>
#include <imgui_impl_sdlrenderer3.h>
#include <imgui_impl_sdl3.h>
#include <imgui_internal.h>

#include <memory>
#include <vector>

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
    class Context;
    class Core final {
    public:
        Core() = default;
        ~Core();
    public:
        bool Init();
    private:
        bool Init_Config();
        bool Init_Spdlog();
        bool Init_SDL();
        bool Init_Timer();
        bool Init_InputManager();
        bool Init_ResourceManager();
        bool Init_ImGui();
        bool Init_Context();
        bool Init_SceneManager();
        bool Init_Icon();
    public:
        SDL_AppResult Input(SDL_Event& event);
        void Update();
        void Render();
        void Clean();
    public:
        bool Get_RunningState()const;
        SDL_AppResult Get_AppResult()const;
        datetime::Timer& Get_Timer();
    private:
        static void Quit_TryCallback(void* userdata, SDL_TrayEntry* entry);
    private:
        ImGuiID dockspaceId;
        ImVec4 backColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    private:
        std::unique_ptr<Config> config_;
        std::unique_ptr<Context> context_;
        std::unique_ptr<datetime::Timer> timer_;
        std::unique_ptr<input::InputManager> inputManager_;
        std::unique_ptr<resource::ResourceManager> resourceManager_;
        std::unique_ptr<scene::SceneManager> sceneManager_;
        std::vector<SDL_TrayCallback> trayCallback;
    private:
        raii::SDL_WindowPtr window_;
        raii::SDL_TrayType trayType_;
        raii::SDL_RendererPtr renderer_;
        /* SDL_AppResult */
        bool runningState_ = false;
        bool imguiInitState_ = false;
        std::atomic<SDL_AppResult> appResult_ = SDL_APP_CONTINUE;
        bool updateState_ = false;
    private:
        const char* Get_ClassName() const {
            return typeid(*this).name();
        }
    };
}