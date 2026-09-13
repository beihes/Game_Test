#include "../inc/SceneManager.hpp"
#include "../inc/Scene.hpp"
#include "core/inc/Context.hpp"
#include <spdlog/spdlog.h>

namespace engine::scene {

    SceneManager::SceneManager(engine::core::Context& context)
        : context_(context) {
        spdlog::trace("[{}]SceneManager 构建完成", this->Get_ClassName());
    }

    SceneManager::~SceneManager() {
        this->Clean(); // 即使不手动调用 close 也能确保清理
        spdlog::trace("[{}]SceneManager 析构完成", this->Get_ClassName());
    }

    Scene* SceneManager::Get_CurrentScene() const {
        if (this->sceneStack_.empty()) { return nullptr; }
        return this->sceneStack_.back().get(); // 返回栈顶场景的裸指针
    }

    void SceneManager::Request_Pop_Scene() {
        this->pendingAction_ = PendingAction::Pop;
    }

    void SceneManager::Request_Replace_Scene(std::unique_ptr<Scene>&& scene) {
        this->pendingAction_ = PendingAction::Replace;
        this->pendingScene_ = std::move(scene);
    }

    void SceneManager::Request_Push_Scene(std::unique_ptr<Scene>&& scene) {
        this->pendingAction_ = PendingAction::Push;
        this->pendingScene_ = std::move(scene);
    }

    // --- Private Methods ---

    void SceneManager::ProcessPendingActions() {
        if (this->pendingAction_ == PendingAction::None) {
            return;
        }
        switch (this->pendingAction_) {
            case PendingAction::Pop:        this->Pop_Scene(); break;
            case PendingAction::Replace:    this->Replace_Scene(std::move(pendingScene_)); break;
            case PendingAction::Push:       this->Push_Scene(std::move(pendingScene_)); break;
            default:                        break;
        }
        this->pendingAction_ = PendingAction::None;
    }

    void SceneManager::Push_Scene(std::unique_ptr<Scene>&& scene) {
        if (!scene) {
            spdlog::trace("[{}]尝试将空场景压入栈。", this->Get_ClassName());
            return;
        }
        spdlog::trace("[{}]正在将场景 '{}' 压入栈。", this->Get_ClassName(), scene->Get_Name());
        this->sceneStack_.push_back(std::move(scene));/* 将新场景移入栈顶 */
        this->sceneStack_.back()->Init();/* 初始化新场景 */
    }

    void SceneManager::Pop_Scene() {
        if (this->sceneStack_.empty()) {
            spdlog::trace("[{}]尝试从空场景栈中弹出", this->Get_ClassName());
            return;
        }
        spdlog::trace("[{}]正在从栈中弹出场景 '{}'", this->Get_ClassName(), this->sceneStack_.back()->Get_Name());
        // 清理并移除栈顶场景
        if (this->sceneStack_.back()) { this->sceneStack_.back()->Clean(); }// 显式调用清理
        this->sceneStack_.pop_back();
    }

    void SceneManager::Replace_Scene(std::unique_ptr<Scene>&& scene) {
        if (!scene) {
            spdlog::trace("[{}]尝试用空场景替换。", this->Get_ClassName());
            return;
        }
        spdlog::trace("[{}]正在用场景 '{}' 替换场景 '{}' 。", this->Get_ClassName(), scene->Get_Name(), this->sceneStack_.back()->Get_Name());
        // 清理并移除场景栈中所有场景
        while (!this->sceneStack_.empty()) {
            if (this->sceneStack_.back()) { this->sceneStack_.back()->Clean(); }
            this->sceneStack_.pop_back();
        }
        // 初始化新场景
        if (!scene->Get_InitState()) { scene->Init(); }
        // 将新场景压入栈顶
        this->sceneStack_.push_back(std::move(scene));
    }

    void SceneManager::Update(float deltaTime) {
        // 只更新栈顶（当前）场景
        Scene* currentScene = this->Get_CurrentScene();
        if (currentScene) { currentScene->Update(deltaTime); }
        // 执行可能的切换场景操作
        this->ProcessPendingActions();
    }

    void SceneManager::Render() {
        // 渲染时需要叠加渲染所有场景，而不只是栈顶
        for (const auto& scene : this->sceneStack_) {
            if (scene) { scene->Render(); }
        }
    }

    void SceneManager::Input(const SDL_Event& event) {
        // 只考虑栈顶场景
        Scene* currentScene = this->Get_CurrentScene();
        if (currentScene) { currentScene->Input(event); }
    }

    void SceneManager::Clean() {
        spdlog::trace("[{}]正在关闭场景管理器并清理场景栈...", this->Get_ClassName());
        // 清理栈中所有剩余的场景（从顶到底）
        while (!this->sceneStack_.empty()) {
            if (this->sceneStack_.back()) {
                spdlog::trace("[{}]正在清理场景 '{}' 。", this->Get_ClassName(), this->sceneStack_.back()->Get_Name());
                this->sceneStack_.back()->Clean();
            }
            this->sceneStack_.pop_back();
        }
    }

} // namespace engine::scene