#include "../inc/Scene.hpp"
#include "core/inc/Context.hpp"
#include "object/inc/Object.hpp"
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
        bool midNeedRemoveState = false;
        for (auto& obj : this->objects_) {
            if (!obj) {
                spdlog::warn("[{}]尝试更新一个空的游戏对象指针。", this->Get_ClassName());
                continue;
            }
            if (!obj->Get_NeedRemoveState()) { obj->Update(deltaTime, context_); } else {
                midNeedRemoveState = true;
                obj->Clean();
            }
        }
        if (midNeedRemoveState) {
            std::erase_if(this->objects_, [](const std::unique_ptr<object::Object>& obj) {
                return !obj || obj->Get_NeedRemoveState(); });
        }
        this->ProcessPendingAdditions();      // 处理待添加（延时添加）的游戏对象
        this->uiManager_->Update(deltaTime);
    }

    void Scene::Render() {
        if (!this->initState_) return;
        // 渲染UI管理器
        for (const auto& obj : this->objects_) {
            if (obj) obj->Render(context_);
        }
        this->uiManager_->Render();
    }

    void Scene::Input(const SDL_Event& event) {
        if (!this->initState_) { return; }
        // 处理UI管理器输入
        this->uiManager_->Input(event);
        for (auto& obj : this->objects_) {
            if (obj && !obj->Get_NeedRemoveState()) { obj->Input(context_); }
        }
           // 不在这里移除，以免浪费算力。在update中移除
    }

    void Scene::Clean() {
        if (!this->initState_) return;
        this->uiManager_->Clean();
        for (const auto& obj : this->objects_) {
            if (obj) obj->Clean();
        }
        this->objects_.clear();
        this->initState_ = false;        // 清理完成后，设置场景为未初始化
        spdlog::trace("[{}]场景 '{}' 清理完成。", this->Get_ClassName(), this->sceneName_);
    }

    void Scene::Add_Object(std::unique_ptr<object::Object>&& object) {
        if (object) this->objects_.push_back(std::move(object));
        else spdlog::warn("[{}]尝试向场景 '{}' 添加空游戏对象。", this->Get_ClassName(), this->sceneName_.c_str());
    }

    void Scene::Safe_Add_Object(std::unique_ptr<object::Object>&& object) {
        if (object) this->pendingAdditions_.push_back(std::move(object));
        else spdlog::warn("[{}]尝试向场景 '{}' 添加空游戏对象。", this->Get_ClassName(), this->sceneName_.c_str());
    }

    void Scene::Remove_Object(object::Object* objectPtr) {
        if (!objectPtr) {
            spdlog::warn("[{}]尝试从场景 '{}' 中移除一个空的游戏对象指针。", this->Get_ClassName(), this->sceneName_.c_str());
            return;
        }
        // erase-remove 移除法不可用，因为智能指针与裸指针无法比较
        // 需要使用 std::remove_if 和 lambda 表达式自定义比较的方式
        auto it = std::remove_if(this->objects_.begin(), this->objects_.end(),
            [objectPtr](const std::unique_ptr<object::Object>& p) {
                return p.get() == objectPtr;    // 比较裸指针是否相等（自定义比较方式）
            });

        if (it != this->objects_.end()) {
            (*it)->Clean();             // 因为传入的是指针，因此只可能有一个元素被移除，不需要遍历it到末尾
            this->objects_.erase(it, this->objects_.end());   // 删除从it到末尾的元素（最后一个元素）
            spdlog::trace("[{}]从场景 '{}' 中移除游戏对象。", this->Get_ClassName(), this->sceneName_.c_str());
        } else {
            spdlog::warn("[{}]游戏对象指针未找到在场景 '{}' 中。", this->Get_ClassName(), this->sceneName_.c_str());
        }
    }

    void Scene::Safe_Remove_Object(object::Object* objectPtr) {
        objectPtr->Set_NeedRemoveState(true);
    }

    object::Object* Scene::Search_Object(const std::string& name) const {
        // 找到第一个符合条件的游戏对象就返回
        for (const auto& obj : this->objects_) {
            if (obj && obj->Get_Name() == name) { return obj.get(); }
        }
        return nullptr;
    }

    void Scene::ProcessPendingAdditions() {
        // 处理待添加的游戏对象
        for (auto& game_object : this->pendingAdditions_) {
            this->Add_Object(std::move(game_object));
        }
        this->pendingAdditions_.clear();
    }

} // namespace engine::scene 