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

#include <entt/entt.hpp>
#include <memory>
#include <vector>
#include <typeinfo>

namespace engine::core {
    class Context;
}

namespace engine::ui {
    class UiManager;
}

namespace engine::scene {
    class SceneManager;
    /**
     * @brief 场景基类，负责管理场景中的游戏对象和场景生命周期。
     *
     * 包含一组游戏对象，并提供更新、渲染、处理输入和清理的接口。
     * 派生类应实现具体的场景逻辑。
     */
    class Scene {
    public:
        /**
        * @brief 构造函数。
        *
        * @param name 场景的名称。
        * @param context 场景上下文。
        * @param scene_manager 场景管理器。
        */
        Scene(std::string_view name, core::Context& context, scene::SceneManager& sceneManager);
        virtual ~Scene();    // 1. 基类必须声明虚析构函数才能让派生类析构函数被正确调用。
        // 2. 析构函数定义必须写在cpp中，不然需要引入Object头文件
    public:
        //删除拷贝和移动构造函数及赋值运算符
        Scene(const Scene&) = delete;
        Scene& operator=(const Scene&) = delete;
        Scene(Scene&&) = delete;
        Scene& operator=(Scene&&) = delete;
    public:
        // getters and setters
        void Set_Name(std::string_view name) { this->sceneName_ = name; }               ///< @brief 设置场景名称
        const std::string& Get_Name() const { return this->sceneName_; }                  ///< @brief 获取场景名称
        void Set_InitState(bool initState) { this->initState_ = initState; }    ///< @brief 设置场景是否已初始化
        bool Get_InitState() const { return this->initState_; }                      ///< @brief 获取场景是否已初始化
    public:
        entt::registry& Get_Registry() { return this->registry_; }
        std::vector<entt::entity>& Get_EntityVector() { return this->entityVector_; }
    public:
        core::Context& Get_Context() const { return context_; }                  ///< @brief 获取上下文引用
        ui::UiManager* Get_UiManager()const { return this->uiManager_.get(); };
    public:
        // 核心循环方法
        virtual void Init();                        ///< @brief 初始化场景。
        virtual void Update(float deltaTime);      ///< @brief 更新场景。
        virtual void Render();                      ///< @brief 渲染场景。
        virtual void Input(const SDL_Event& event);                       ///< @brief 处理输入。
        virtual void Clean();                       ///< @brief 清理场景。
    protected:
        std::string sceneName_;                     ///< @brief 场景名称
        std::unique_ptr<ui::UiManager> uiManager_;  ///< @brief UI管理器(初始化时自动创建)
        bool initState_ = false;                       ///< @brief 场景是否已初始化(非当前场景很可能未被删除，因此需要初始化标志避免重复初始化)
    public:
        entt::registry registry_;
        std::vector<entt::entity> entityVector_;
    protected:
        core::Context& context_;                    ///< @brief 上下文引用（隐式，构造时传入）
        SceneManager& sceneManager_;
    private:
        const char* Get_ClassName() const {
            return typeid(*this).name();
        }
    };
}