#include "../inc/AppContext.hpp"
#include "engine/core/inc/Context.hpp"
#include "engine/resource/inc/ResourceManager.hpp"
#include "engine/resource/inc/TextureManager.hpp"
#include "engine/scene/inc/SceneManager.hpp"
#include <spdlog/spdlog.h>
#include <spdlog/fmt/bin_to_hex.h>

namespace app::scene {
    AppContext::AppContext(engine::core::Context& context, engine::scene::SceneManager& sceneManager)
        :context_(context), sceneManager_(sceneManager) {
        spdlog::trace("[{}]AppContext 构建完成", this->Get_ClassName());
    }

    AppContext::~AppContext() {
        spdlog::trace("[{}]AppContext 析构完成", this->Get_ClassName());
    }

    engine::core::Context& AppContext::Get_Context()const {
        return this->context_;
    }

    engine::scene::SceneManager& AppContext::Get_SceneManager()const {
        return this->sceneManager_;
    }

    void AppContext::Set_DockSpaceId(ImGuiID& midId) {
        this->dockSpaceId_ = midId;
    }

    const std::optional<std::reference_wrapper<ImGuiID>> AppContext::Get_DockSpaceId()const {
        return this->dockSpaceId_->get();
    }
} // namespace app::scene