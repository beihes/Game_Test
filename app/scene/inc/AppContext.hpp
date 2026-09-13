#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include <SDL3/SDL.h>
    // #include <SDL3_image/SDL_image.h>
        // #include <SDL3_ttf/SDL_ttf.h>
#ifdef __cplusplus
}
#endif // __cplusplus

#include <imgui.h>
#include <array>
#include <atomic>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <optional>


namespace engine::core {
    class Context;
}

namespace engine::scene {
    class SceneManager;
} // namespace engine::scene

namespace app::scene {
    class AppScene;
    class AppContext {
    public:
        AppContext(AppScene& appScene, engine::core::Context& context, engine::scene::SceneManager& sceneManager);
        ~AppContext();
    public:
        engine::core::Context& Get_Context()const;
        engine::scene::SceneManager& Get_SceneManager()const;
        void Set_DockSpaceId(ImGuiID& midId);
    public:
        const std::optional<std::reference_wrapper<ImGuiID>> Get_DockSpaceId()const;
    public:
        AppScene& appScene_;
    private:
        std::optional<std::reference_wrapper<ImGuiID>> dockSpaceId_;
    private:
        engine::core::Context& context_;
        engine::scene::SceneManager& sceneManager_;
    private:
        const char* Get_ClassName() const {
            return typeid(*this).name();
        }
    };
}