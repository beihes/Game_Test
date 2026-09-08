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

#include <iostream>
#include <vector>
#include <memory>

namespace engine::core {
    class Context;
}

namespace engine::ui {
    class Ui;
    class UiManager {
    public:
        UiManager(core::Context& context);
        ~UiManager();
    public:
        //删除拷贝和移动构造函数及赋值运算符
        UiManager(const UiManager&) = delete;
        UiManager& operator=(const UiManager&) = delete;
        UiManager(UiManager&&) = delete;
        UiManager& operator=(UiManager&&) = delete;
    public:
        // 延时切换场景
        void Request_Push_UI(std::shared_ptr<Ui>&& ui);      ///< @brief 请求压入一个新场景。
        void Request_Pop_UI();                                     ///< @brief 请求弹出当前场景。
        void Request_Replace_UI(std::shared_ptr<Ui>&& ui);   ///< @brief 请求替换当前场景。
    public:
        void ProcessPendingActions();
        void Push_UI(std::shared_ptr<Ui>&& ui);         ///< @brief 将一个新UI压入栈顶，使其成为活动UI。
        void Pop_UI();                                     ///< @brief 移除栈顶UI。
        void Replace_UI(std::shared_ptr<Ui>&& ui);      ///< @brief 清理场景栈所有UI，将此UI设为栈顶UI。
    public:
        // 核心循环函数
        void Update(float deltaTime);
        void Render();
        void Input(const SDL_Event& event);
        void Clean();
    public:
        Ui* Search_UI(std::string_view name)const;
    private:
        std::vector<std::shared_ptr<Ui>> uiStack_;
        std::shared_ptr<Ui> pendingUI_;                 ///< @brief 待处理UI
        enum class PendingAction { None, Push, Pop, Replace };  ///< @brief 待处理的动作
        PendingAction pendingAction_ = PendingAction::None;    ///< @brief 待处理的动作
        bool initState_ = false;
    private:
        core::Context& context_;
    private:
        const char* Get_ClassName() const {
            return typeid(*this).name();
        }
    };
}