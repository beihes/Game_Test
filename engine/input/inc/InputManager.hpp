#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include <SDL3/SDL.h>
#ifdef __cplusplus
}
#endif // __cplusplus

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <optional>
#include <string>
#include <unordered_map>
#include <variant>

namespace engine::core {
    class Config;
    class Context;
}

namespace engine::input {
    enum class ActionState {
        PRESSED,    //动作在本帧按下
        HELD,       //动作被持续按下
        RELEASED,   //动作在本帧释放
        INACTIVE    //动作未激活
    };
    class InputManager final {
    public:
        InputManager();
        ~InputManager();
    public:
        //删除拷贝和移动构造函数及赋值运算符
        InputManager(const InputManager&) = delete;
        InputManager& operator=(const InputManager&) = delete;
        InputManager(InputManager&&) = delete;
        InputManager& operator=(InputManager&&) = delete;
    public:
        bool Init(core::Context& context);
        void Input(const SDL_Event& event);
        void Update();
        void Clear();
    public:
        bool IsActionDown(const std::string& actionName)const;
        bool IsActionPressed(const std::string& actionName)const;
        bool isActionReleased(const std::string& actionName)const;
        bool ShouldQuit()const;
        const glm::vec2& Get_MousePosition()const;//获取屏幕坐标
        glm::vec2 Get_LogicalMousePosition()const;//获取逻辑坐标
    public:
        void Set_RunningState(bool runningState);
        void Set_AppResult(SDL_AppResult appResult);
    public:
        entt::sink<entt::sigh<void()>> OnAction(const std::string& actionName, ActionState actionState = ActionState::PRESSED);
        void Add_Action(const std::string& actionName, const std::string& keyName);
    private:
        void Process_Event(const SDL_Event& event);
        void Init_KeyMap();
        void Update_ActionState(const std::string& actionName, bool isInputActive, bool isRepeatEvent);
        SDL_Scancode Get_Scancode_From_String(const std::string& keyName);
        Uint8 Get_MouseButtonUint8_From_String(const std::string& buttonName);
    private:
        std::optional<std::reference_wrapper<core::Context>> context_;
        glm::vec2 mousePosition_;
    private:
        std::unordered_map<std::string, std::array<entt::sigh<void()>, 3>> actionsToFunc_;
        std::unordered_map<std::variant<SDL_Scancode, Uint8>, std::vector<std::string>> inputToActions_ = {};//从建盘或鼠标获取动作名称列表
        std::unordered_map<std::string, ActionState> actionStates_ = {};//存储每个动作当前状态
    private:
        const char* Get_ClassName() const {
            return typeid(*this).name();
        }
    };
}
