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

#include "engine/scene/inc/Scene.hpp"
#include <imgui.h>
#include <imgui_internal.h>
#include <iostream>
#include <vector>
#include <memory>

namespace app::device {
    namespace light {
    } // namespace light
} // namespace app::control

namespace engine::core {
    class Context;
}

namespace app::scene {
    class AppContext;
    class AppId;
    class AppScene final : public engine::scene::Scene {
    public:
        AppScene(std::string_view name, engine::core::Context& context, engine::scene::SceneManager& sceneManager);
        ~AppScene()override;
    public:
        // 核心循环方法
        void Init()override;                        ///< @brief 初始化场景。
        void Update(float deltaTime)override;       ///< @brief 更新场景。
        void Render()override;                      ///< @brief 渲染场景。
        void Input(const SDL_Event& event)override; ///< @brief 处理输入。
        void Clean()override;                       ///< @brief 清理场景。
    private:
        void OnAttack();
        void OnJump();
    private:
        ImGuiID dockSpaceId_ = 0;
        ImGuiDockNode* dockSpaceNode_ = nullptr;
        ImGuiID dockIdLeft = 0;
        ImGuiID dockIdMain = 0;
        std::unique_ptr<AppContext> appContext_;
    private:
        const char* Get_ClassName() const {
            return typeid(*this).name();
        }
    };
}