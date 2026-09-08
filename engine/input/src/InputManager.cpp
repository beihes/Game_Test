#include "../inc/InputManager.hpp"
#include "core/inc/Config.hpp"
#include "core/inc/Context.hpp"
#include <spdlog/spdlog.h>

namespace engine::input {
    InputManager::InputManager() {
        spdlog::trace("[{}]InputManager 构建成功", this->Get_ClassName());
    }

    InputManager::~InputManager() {
        this->Clear();
        spdlog::trace("[{}]InputManager 析构成功", this->Get_ClassName());
    }

    bool InputManager::Init(core::Context& context) {
        this->context_ = context;
        this->Init_KeyMap();
        SDL_GetMouseState(&this->mousePosition_.x, &this->mousePosition_.y);
        spdlog::trace("[{}]初始鼠标位置 ({},{})", this->Get_ClassName(), this->mousePosition_.x, this->mousePosition_.y);
        return true;
    }

    void InputManager::Input(const SDL_Event& event) {
        for (auto& [actionName, state] : actionStates_) {
            if (state == ActionState::PRESSED) {
                state = ActionState::HELD;
            } else if (state == ActionState::RELEASED) { state = ActionState::INACTIVE; }
        }
        this->Process_Event(event);
        for (auto& [actionNameId, state] : this->actionStates_) {
            if (state != ActionState::INACTIVE) {
                if (auto it = this->actionsToFunc_.find(actionNameId); it != this->actionsToFunc_.end()) {
                    it->second.at(static_cast<size_t>(state)).publish();
                }
            }
        }
    }

    void InputManager::Update() {

    }

    void InputManager::Clear() {

    }

    bool InputManager::IsActionDown(const std::string& actionName) const {
        if (auto it = this->actionStates_.find(actionName); it != this->actionStates_.end()) {
            return it->second == ActionState::PRESSED || it->second == ActionState::HELD;
        }
        return false;
    }

    bool InputManager::IsActionPressed(const std::string& actionName) const {
        if (auto it = this->actionStates_.find(actionName); it != this->actionStates_.end()) {
            return it->second == ActionState::PRESSED;
        }
        return false;
    }

    bool InputManager::isActionReleased(const std::string& actionName) const {
        if (auto it = this->actionStates_.find(actionName); it != this->actionStates_.end()) {
            return it->second == ActionState::RELEASED;
        }
        return false;
    }

    bool InputManager::ShouldQuit() const {
        return this->context_->get().Get_RunningState();
    }
    const glm::vec2& InputManager::Get_MousePosition() const {
        return this->mousePosition_;
    }
    glm::vec2 InputManager::Get_LogicalMousePosition() const {
        glm::vec2 midPoint;
        SDL_RenderCoordinatesFromWindow(this->context_->get().Get_Renderer(), this->mousePosition_.x, this->mousePosition_.y, &midPoint.x, &midPoint.y);
        return midPoint;
    }

    void InputManager::Set_RunningState(bool runningState) {
        this->context_->get().Get_RunningState() = runningState;
    }

    void InputManager::Set_AppResult(SDL_AppResult appResult) {
        this->context_->get().Get_AppResult().store(appResult);
    }

    entt::sink<entt::sigh<void()>> InputManager::OnAction(const std::string& actionName, ActionState actionState) {
        return this->actionsToFunc_[actionName].at(static_cast<size_t>(actionState));
    }

    void InputManager::Add_Action(const std::string& actionName, const std::string& keyName) {
        this->actionStates_[actionName] = ActionState::INACTIVE;
        SDL_Scancode scancode = this->Get_Scancode_From_String(keyName);
        Uint8 mouseButton = this->Get_MouseButtonUint8_From_String(keyName);
        if (scancode != SDL_SCANCODE_UNKNOWN) {
            this->inputToActions_[scancode].push_back(actionName);
            spdlog::trace("[{}]映射按键：{}(SDL_Scancode: {})到动作：{}", this->Get_ClassName(), keyName, static_cast<uint16_t>(scancode), actionName);
        } else if (mouseButton != 0) {
            this->inputToActions_[mouseButton].push_back(actionName);
            spdlog::trace("[{}]映射鼠标按钮：{}(Uint8: {})到动作：{}", this->Get_ClassName(), keyName, mouseButton, actionName);
        } else {
            spdlog::warn("[{}]输入映射警告：未知按钮名称'{}'到动作'{}'", this->Get_ClassName(), keyName, actionName);
        }
    }

    void InputManager::Process_Event(const SDL_Event& event) {
        switch (event.type) {
            case SDL_EVENT_KEY_DOWN:
            case SDL_EVENT_KEY_UP:
            {
                SDL_Scancode scancode = event.key.scancode;
                bool isDown = event.key.down;
                bool isRepeat = event.key.repeat;
                auto it = this->inputToActions_.find(scancode);
                if (it != this->inputToActions_.end()) {
                    const std::vector<std::string>& associatedActions = it->second;
                    for (const std::string& actionName : associatedActions) {
                        this->Update_ActionState(actionName, isDown, isRepeat);
                    }
                }
                break;
            }
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
            case SDL_EVENT_MOUSE_BUTTON_UP:
            {
                Uint8 button = event.button.button;
                bool isDown = event.button.down;
                auto it = this->inputToActions_.find(button);
                if (it != this->inputToActions_.end()) {
                    const std::vector<std::string>& associatedActions = it->second;
                    for (const std::string& actionName : associatedActions) {
                        this->Update_ActionState(actionName, isDown, false);
                    }
                }
                this->mousePosition_ = { event.button.x,event.button.y };
                break;
            }
            case SDL_EVENT_MOUSE_MOTION:
                this->mousePosition_ = { event.motion.x,event.motion.y };
                break;
            case SDL_EVENT_QUIT:
                this->context_->get().Get_RunningState() = false;
                this->Set_AppResult(SDL_APP_SUCCESS);
                break;
            default:
                break;
        }
    }
    void InputManager::Init_KeyMap() {
        spdlog::trace("[{}]初始化输入映射", this->Get_ClassName());
        auto actionsToKeyname = this->context_->get().Get_Config().Get_KeyMap();
        this->inputToActions_.clear();
        this->actionStates_.clear();
        if (actionsToKeyname.find("MouseLeft") == actionsToKeyname.end()) {
            spdlog::trace("[{}]配置中没有定义'MouseLeft'", this->Get_ClassName());
            actionsToKeyname["MouseLeft"] = { "MouseLeft" };
        }
        if (actionsToKeyname.find("MouseRight") == actionsToKeyname.end()) {
            spdlog::trace("[{}]配置中没有定义'MouseRight'", this->Get_ClassName());
            actionsToKeyname["MouseRight"] = { "MouseRight" };
        }
        for (const auto& [actionName, keyNames] : actionsToKeyname) {
            this->actionStates_[actionName] = ActionState::INACTIVE;
            spdlog::trace("[{}]映射动作: {}", this->Get_ClassName(), actionName);
            for (const std::string& keyName : keyNames) {
                SDL_Scancode scancode = this->Get_Scancode_From_String(keyName);
                Uint8 mouseButton = this->Get_MouseButtonUint8_From_String(keyName);
                if (scancode != SDL_SCANCODE_UNKNOWN) {
                    this->inputToActions_[scancode].push_back(actionName);
                    spdlog::trace("[{}]映射按键：{}(SDL_Scancode: {})到动作：{}", this->Get_ClassName(), keyName, static_cast<uint16_t>(scancode), actionName);
                } else if (mouseButton != 0) {
                    this->inputToActions_[mouseButton].push_back(actionName);
                    spdlog::trace("[{}]映射鼠标按钮：{}(Uint8: {})到动作：{}", this->Get_ClassName(), keyName, mouseButton, actionName);
                } else {
                    spdlog::warn("[{}]输入映射警告：未知按钮名称'{}'到动作'{}'", this->Get_ClassName(), keyName, actionName);
                }
            }
        }
        spdlog::trace("[{}]输入映射初始化完成", this->Get_ClassName());
    }

    void InputManager::Update_ActionState(const std::string& actionName, bool isInputActive, bool isRepeatEvent) {
        auto it = this->actionStates_.find(actionName);
        if (it == this->actionStates_.end()) {
            spdlog::warn("[{}]尝试更新未注册的动作状态: {}", this->Get_ClassName(), actionName);
            return;
        }
        if (isInputActive) {
            if (isRepeatEvent)it->second = ActionState::HELD;
            else it->second = ActionState::PRESSED;
        } else it->second = ActionState::RELEASED;
    }

    SDL_Scancode InputManager::Get_Scancode_From_String(const std::string& keyName) {
        return SDL_GetScancodeFromName(keyName.c_str());
    }

    Uint8 InputManager::Get_MouseButtonUint8_From_String(const std::string& buttonName) {
        if (buttonName == "MouseLeft")return SDL_BUTTON_LEFT;
        if (buttonName == "MouseMiddle")return SDL_BUTTON_MIDDLE;
        if (buttonName == "MouseRight")return SDL_BUTTON_RIGHT;
        if (buttonName == "MouseX1")return SDL_BUTTON_X1;
        if (buttonName == "MouseX2")return SDL_BUTTON_X2;
        return 0;
    }
}
