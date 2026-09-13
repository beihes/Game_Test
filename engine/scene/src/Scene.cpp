#include "../inc/Scene.hpp"
#include "core/inc/Context.hpp"
#include "ui/inc/UiManager.hpp"
#include <spdlog/spdlog.h>
#include <algorithm> // for std::remove_if

namespace engine::scene {

    Scene::Scene(std::string_view name, engine::core::Context& context, engine::scene::SceneManager& sceneManager)
        : sceneName_(name), context_(context), sceneManager_(sceneManager), initState_(false) {
        spdlog::trace("[{}]Scene 构建完成: '{}'", this->Get_ClassName(), this->sceneName_);
    }

    Scene::~Scene() {
        spdlog::trace("[{}]Scene 析构完成: '{}'", this->Get_ClassName(), this->sceneName_);
    }

    void Scene::Init() {
        this->uiManager_ = std::make_unique<ui::UiManager>(this->context_);
        if (!this->uiManager_) {
            spdlog::trace("[{}]场景 '{}' 的 UiManager 创建失败。", this->Get_ClassName(), this->sceneName_);
            return;
        }

        this->initState_ = true;     // 子类应该最后调用父类的 init 方法
        spdlog::trace("[{}]场景 '{}' 初始化完成。", this->Get_ClassName(), this->sceneName_);
    }

    void Scene::Update(float deltaTime) {
        if (!this->initState_) return;
        this->uiManager_->Update(deltaTime);
    }

    void Scene::Render() {
        if (!this->initState_) return;
        this->uiManager_->Render();
    }

    void Scene::Input(const SDL_Event& event) {
        if (!this->initState_) { return; }
        // 处理UI管理器输入
        this->uiManager_->Input(event);
    }

    void Scene::Clean() {
        if (!this->initState_) return;
        this->uiManager_->Clean();
        this->initState_ = false;        // 清理完成后，设置场景为未初始化
        spdlog::trace("[{}]场景 '{}' 清理完成。", this->Get_ClassName(), this->sceneName_);
    }
} // namespace engine::scene 