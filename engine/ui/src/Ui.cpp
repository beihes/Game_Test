#include "../inc/Ui.hpp"
#include <spdlog/spdlog.h>

namespace engine::ui {
    Ui::Ui(std::string_view name, core::Context& context)
        :name_(name), context_(context) {
        spdlog::trace("[{}]Ui 构建完成: '{}'", this->Get_ClassName(), this->name_);
    }

    Ui::~Ui() {
        spdlog::trace("[{}]Ui 析构完成: '{}'", this->Get_ClassName(), this->name_);
    }

    void Ui::Init() {
        this->runningState_ = true;
        this->initState_ = true;
        spdlog::trace("[{}]Ui '{}' 初始化完成。", this->Get_ClassName(), this->name_);
    }
    void Ui::Input([[maybe_unused]] const SDL_Event& event) {
        if (!this->initState_) {
            return;
        }
        if (!this->runningState_) { return; }
    }
    void Ui::Render() {
        if (!this->initState_ && !this->runningState_) { return; };
    }
    void Ui::Update() {
        if (!this->initState_ && !this->runningState_) { return; };
    }
    void Ui::Clean() {
        if (!this->initState_) return;
        this->initState_ = false;
        spdlog::trace("[{}]Ui '{}' 清理完成。", this->Get_ClassName(), this->name_);
    }
}