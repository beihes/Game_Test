#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include <SDL3/SDL.h>
    // #include <SDL3_image/SDL_image.h>
        //#include <SDL3_ttf/SDL_ttf.h>
#ifdef __cplusplus
}
#endif // __cplusplus

#include <imgui.h>
#include <string>
#include <vector>
#include <typeinfo>

namespace engine::core {
    class Context;
}

namespace engine::ui {
    class Ui {
    public:
        Ui(std::string_view name, core::Context& context);
        virtual ~Ui();
    public:
        void Set_Name(std::string_view name) { this->name_ = name; }               ///< @brief 设置场景名称
        const std::string Get_Name() const { return this->name_; }                  ///< @brief 获取场景名称
        void Set_InitState(bool initialized) { this->initState_ = initialized; }    ///< @brief 设置场景是否已初始化
        bool Get_InitState() const { return this->initState_; }                      ///< @brief 获取场景是否已初始化
        void Set_WindowFlags(ImGuiWindowFlags midWindowFlags) { this->windowFlags_ = midWindowFlags; }
        ImGuiWindowFlags Get_WindowFlags()const { return this->windowFlags_; }
    public:
        virtual void Init();
        virtual void Input(const SDL_Event& event);
        virtual void Render();
        virtual void Update();
        virtual void Clean();
    protected:
        bool runningState_ = false;// 程序是否运行
    protected:
        std::string name_;
        bool initState_ = false;
        ImGuiWindowFlags windowFlags_ = ImGuiWindowFlags_None;
    protected:
        core::Context& context_;
    private:
        const char* Get_ClassName() const {
            return typeid(*this).name();
        }
    };
}