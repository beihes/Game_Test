#include "../inc/Context.hpp"
#include "../inc/Config.hpp"
#include "input/inc/InputManager.hpp"
#include "resource/inc/ResourceManager.hpp"
#include <spdlog/spdlog.h>
#include <atomic>

namespace engine::core {
    Context::Context(Config& config, input::InputManager& inputManager, resource::ResourceManager& resourceManager, SDL_Window* window, sdl::SDL_TrayType& trayType, SDL_Renderer* renderer, bool& runningState, std::atomic<SDL_AppResult>& appResult)
        :config_(config), inputManager_(inputManager), resourceManager_(resourceManager), window_(window), trayType_(trayType), renderer_(renderer), runningState_(runningState), appResult_(appResult) {
        spdlog::trace("[{}]Context 构建完成", this->Get_ClassName());
    }

    Context::~Context() {
        spdlog::trace("[{}]Context 析构完成", this->Get_ClassName());
    }

    Config& Context::Get_Config() const {
        return this->config_;
    }

    input::InputManager& Context::Get_InputManager()const {
        return this->inputManager_;
    }

    resource::ResourceManager& Context::Get_ResourceManager() const {
        return this->resourceManager_;
    }

    SDL_Window* Context::Get_Window() const {
        return this->window_;
    }

    SDL_Renderer* Context::Get_Renderer()const {
        return this->renderer_;
    }

    bool& Context::Get_RunningState()const {
        return this->runningState_;
    }

    std::atomic<SDL_AppResult>& Context::Get_AppResult()const {
        return this->appResult_;
    }
}