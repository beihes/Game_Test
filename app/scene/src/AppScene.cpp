#include "../inc/AppScene.hpp"
#include "../inc/AppContext.hpp"
#include "engine/core/inc/Config.hpp"
#include "engine/core/inc/Context.hpp"
#include "engine/input/inc/InputManager.hpp"
#include "engine/ui/inc/UIManager.hpp"
#include "engine/scene/inc/SceneManager.hpp"
#include <imgui.h>
#include <spdlog/spdlog.h>

namespace app::scene {
    AppScene::AppScene(std::string_view name, engine::core::Context& context, engine::scene::SceneManager& sceneManager)
        :Scene(name, context, sceneManager) {
        spdlog::trace("[{}]AppScene 构建完成", this->Get_ClassName());
    }

    AppScene::~AppScene() {
        spdlog::trace("[{}]AppScene 析构完成", this->Get_ClassName());
    }

    void AppScene::Init() {
        Scene::Init();
        this->appContext_ = std::make_unique<AppContext>(this->context_, this->sceneManager_);
        if (!this->appContext_) {
            spdlog::error("[{}]AppContext 创建失败", this->Get_ClassName());
            this->context_.Get_RunningState() = false;
            this->context_.Get_AppResult().store(SDL_APP_FAILURE);
            return;
        }
        auto& inputManager = this->context_.Get_InputManager();
        inputManager.Add_Action("Attack", "K");
        inputManager.OnAction("Attack").connect<&AppScene::OnAttack>(this);
        inputManager.Add_Action("Jump", "SPACE");
        inputManager.OnAction("Jump", engine::input::ActionState::RELEASED).connect<&AppScene::OnJump>(this);
    }

    void AppScene::Update(float deltaTime) {
        Scene::Update(deltaTime);
    }

    void AppScene::Render() {
        if (this->dockSpaceId_ == 0) {
            /* 获取或创建一个 Dockspace ID */
            this->dockSpaceId_ = ImGui::GetID(this->Get_Name().c_str());
            this->appContext_->Set_DockSpaceId(this->dockSpaceId_);
        }
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        if ((this->dockSpaceNode_ = ImGui::DockBuilderGetNode(this->dockSpaceId_)) == nullptr) {/* 检查并初始化布局 */
            /* 添加一个停靠节点 */
            ImGui::DockBuilderAddNode(this->dockSpaceId_, ImGuiDockNodeFlags_DockSpace);
            ImGui::DockBuilderSetNodeSize(this->dockSpaceId_, ImGui::GetMainViewport()->WorkSize);
                                                                        /* 固定的宽度/视口的宽度 */
            ImGui::DockBuilderSplitNode(this->dockSpaceId_, ImGuiDir_Left, 55.f / viewport->Size.x, &dockIdLeft, &dockIdMain);
            ImGui::DockBuilderDockWindow("导航栏", dockIdLeft);
            ImGui::DockBuilderDockWindow("主界面", dockIdMain);
            ImGui::DockBuilderFinish(this->dockSpaceId_);/* 完成构建 */
        } else {
        }
        /* 在每一帧，仍然需要提交 DockSpace */
        ImGui::DockSpaceOverViewport(this->dockSpaceId_, viewport);/* 这行代码会在主视口创建一个停靠空间 */
        this->dockSpaceNode_ = ImGui::DockBuilderGetNode(this->dockSpaceId_);
        if (this->dockSpaceNode_) {
            this->dockSpaceNode_->LocalFlags |= ImGuiDockNodeFlags_NoDockingSplit | static_cast<ImGuiDockNodeFlags_>(ImGuiDockNodeFlags_NoTabBar);
        }
        Scene::Render();
    }

    void AppScene::Input(const SDL_Event& event) {
        Scene::Input(event);
    }

    void AppScene::Clean() {
        Scene::Clean();
    }

    void AppScene::OnAttack() {
        spdlog::debug("[{}]玩家发动攻击", this->Get_ClassName());
    }

    void AppScene::OnJump() {
        spdlog::debug("[{}]玩家跳跃", this->Get_ClassName());
    }
}