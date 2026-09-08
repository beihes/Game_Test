#include "../inc/UiManager.hpp"
#include "../inc/Ui.hpp"
#include <spdlog/spdlog.h>

namespace engine::ui {
    UiManager::UiManager(core::Context& context)
        :context_(context) {
        spdlog::trace("[{}]UiManager 构建完成", this->Get_ClassName());
    }

    UiManager::~UiManager() {
        this->Clean(); // 即使不手动调用 close 也能确保清理
        spdlog::trace("[{}]UiManager 析构完成", this->Get_ClassName());
    }

    void UiManager::Request_Push_UI(std::shared_ptr<Ui>&& ui) {
        this->pendingAction_ = PendingAction::Push;
        this->pendingUI_ = std::move(ui);
        this->ProcessPendingActions();
    }

    void UiManager::Request_Pop_UI() {
        this->pendingAction_ = PendingAction::Pop;
        this->ProcessPendingActions();
    }

    void UiManager::Request_Replace_UI(std::shared_ptr<Ui>&& ui) {
        this->pendingAction_ = PendingAction::Replace;
        this->pendingUI_ = std::move(ui);
        this->ProcessPendingActions();
    }

    void UiManager::ProcessPendingActions() {
        if (this->pendingAction_ == PendingAction::None) {
            return;
        }
        switch (this->pendingAction_) {
            case PendingAction::Pop:        this->Pop_UI(); break;
            case PendingAction::Replace:    this->Replace_UI(std::move(this->pendingUI_)); break;
            case PendingAction::Push:       this->Push_UI(std::move(this->pendingUI_)); break;
            default:                        break;
        }
        this->pendingAction_ = PendingAction::None;
    }

    void UiManager::Push_UI(std::shared_ptr<Ui>&& ui) {
        if (!ui) {
            spdlog::trace("[{}]尝试将空UI压入栈。", this->Get_ClassName());
            return;
        }
        spdlog::trace("[{}]正在将UI '{}' 压入栈。", this->Get_ClassName(), ui->Get_Name());
        // 初始化新场景
        if (!ui->Get_InitState()) { ui->Init(); }// 确保只初始化一次
        // 将新场景移入栈顶
        this->uiStack_.push_back(std::move(ui));
    }

    void UiManager::Pop_UI() {
        if (this->uiStack_.empty()) {
            spdlog::trace("[{}]尝试从空UI栈中弹出", this->Get_ClassName());
            return;
        }
        spdlog::trace("[{}]正在从栈中弹出UI '{}'", this->Get_ClassName(), this->uiStack_.back()->Get_Name());
        // 清理并移除栈顶场景
        if (this->uiStack_.back()) { this->uiStack_.back()->Clean(); }// 显式调用清理
        this->uiStack_.pop_back();
    }

    void UiManager::Replace_UI(std::shared_ptr<Ui>&& ui) {
        if (!ui) {
            spdlog::trace("[{}]尝试用空UI替换。", this->Get_ClassName());
            return;
        }
        spdlog::trace("[{}]正在用UI '{}' 替换UI '{}' 。", this->Get_ClassName(), ui->Get_Name(), this->uiStack_.back()->Get_Name());
        // 清理并移除场景栈中所有场景
        while (!this->uiStack_.empty()) {
            if (this->uiStack_.back()) { this->uiStack_.back()->Clean(); }
            this->uiStack_.pop_back();
        }
        if (!ui->Get_InitState()) { ui->Init(); }// 初始化新场景
        this->uiStack_.push_back(std::move(ui));// 将新场景压入栈顶
    }

    void UiManager::Update([[maybe_unused]] float deltaTime) {
        for (const auto& ui : this->uiStack_) {
            if (ui) { ui->Update(); }
        }
        this->ProcessPendingActions();
    }
    void UiManager::Render() {
        for (const auto& ui : this->uiStack_) {
            if (ui) { ui->Render(); }
        }
    }

    void UiManager::Input(const SDL_Event& event) {
        for (const auto& ui : this->uiStack_) {
            if (ui) { ui->Input(event); }
        }
    }

    void UiManager::Clean() {
        for (const auto& ui : this->uiStack_) {
            if (ui) { ui->Clean(); }
        }
    }

    Ui* UiManager::Search_UI(std::string_view name)const {
        if (this->uiStack_.empty()) { return nullptr; }
        auto it = std::find_if(this->uiStack_.begin(), this->uiStack_.end(), [name](const std::shared_ptr<Ui>& midUI) {
            return midUI->Get_Name() == name;
            });
        return it->get();
    }
}