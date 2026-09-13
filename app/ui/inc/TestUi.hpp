#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include <SDL3/SDL.h>
#ifdef __cplusplus
}
#endif // __cplusplus

#include "engine/ui/inc/Ui.hpp"
#include <entt/entt.hpp>
#include <imgui.h>
#include <imgui_internal.h>
#include <array>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace app::scene {
    class AppContext;
}

namespace app::ui {
    class TestUi :public engine::ui::Ui {
    public:
        TestUi(std::string_view name, scene::AppContext& appContext);
        ~TestUi();
    public:
        void Init()override;
        void Input(const SDL_Event& event)override;
        void Render()override;
        void Update()override;
        void Clean()override;
    private:
        scene::AppContext& appContext_;
    private:
        const char* Get_ClassName() const {
            return typeid(*this).name();
        }
    };
} // namespace app::ui
