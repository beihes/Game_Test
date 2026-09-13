#include "../inc/Core.hpp"
#include "../inc/Config.hpp"
#include "../inc/Context.hpp"
#include "input/inc/InputManager.hpp"
#include "resource/inc/ResourceManager.hpp"
#include "resource/inc/SurfaceManager.hpp"
#include "resource/inc/TextureManager.hpp"
#include "scene/inc/Scene.hpp"
#include "scene/inc/SceneManager.hpp"
#include "app/scene/inc/AppScene.hpp"
#include <datetime/datetime.hpp>
#include <font/icons_font_awesome_6.h>
#include <threme/theme.hpp>
#include <implot.h>
#include <implot3d.h>
#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace engine::core {
    Core::~Core() {
        spdlog::trace("[{}]Core 析构完成", this->Get_ClassName());
        spdlog::shutdown();
    }

    bool Core::Init() {
#ifdef _DEBUG
        spdlog::set_level(spdlog::level::trace);
#else
        spdlog::set_level(spdlog::level::info);
#endif
        if (!this->Init_Config()) { return false; }
        if (!this->Init_Spdlog()) { return false; }
        if (!this->Init_SDL()) { return false; }
        if (!this->Init_Timer()) { return false; }
        if (!this->Init_InputManager()) { return false; }
        if (!this->Init_ResourceManager()) { return false; }
        if (!this->Init_ImGui()) { return false; }
        if (!this->Init_Context()) { return false; }
        if (!this->Init_SceneManager()) { return false; }
        if (!this->Init_Icon()) { return false; }
        if (!SDL_ShowWindow(this->window_.get())) {
            spdlog::error("[{}]窗口打开失败", this->Get_ClassName());
            return false;
        }
        this->runningState_ = true;
        spdlog::trace("[{}]Core 初始化成功", this->Get_ClassName());
        return true;
    }

    bool Core::Init_Config() {
        this->config_ = std::make_unique<Config>();
        if (!this->config_) {
            spdlog::error("[{}]Config 初始化失败", this->Get_ClassName());
            return false;
        }
        this->config_->Print_All_Lib_Version();
        return true;
    }

    bool Core::Init_Spdlog() {
        spdlog::init_thread_pool(16384, 1);
        try {
            auto dailySink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(this->config_->logFilePath_, 0, 0);
            auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

            // 3. 创建异步 Logger，并添加多个 Sink
            //    注意：所有 Sink 共享同一个线程池
            std::vector<spdlog::sink_ptr> sinks = { dailySink, consoleSink };
            auto logger = std::make_shared<spdlog::async_logger>(this->config_->loggerName_, sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
            spdlog::set_default_logger(logger); // 设为默认，方便全局调用
#ifdef _DEBUG
            spdlog::set_level(spdlog::level::trace);
#else
            spdlog::set_level(spdlog::level::debug);
#endif
        } catch (const spdlog::spdlog_ex& e) {
            spdlog::error("[{}]spdlog 初始化失败: {}", __func__, e.what());
            return false;
        }
        return true;
    }

    bool Core::Init_SDL() {
        if (!SDL_Init(this->config_->Get_InitFlags())) {
            spdlog::error("[{}]SDL 初始化失败: {}\n", this->Get_ClassName(), SDL_GetError());
            return false;
        }
        this->config_->Set_MainScale(SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay()));
        auto midWindow_Ptr = SDL_CreateWindow(this->config_->Get_WindowTitle().c_str(), (int)(this->config_->Get_WindowSize().x * this->config_->Get_MainScale()), (int)(this->config_->Get_WindowSize().y * this->config_->Get_MainScale()), this->config_->Get_WindowFlags());
        if (!midWindow_Ptr) {
            spdlog::error("[{}]创建 windows 失败: {}", this->Get_ClassName(), SDL_GetError());
            return false;
        }
        this->window_ = raii::SDL_WindowPtr(midWindow_Ptr);
        if (!this->window_) {
            spdlog::error("[{}]创建 windows 失败", this->Get_ClassName());
            raii::SDL_WindowDelete()(midWindow_Ptr);
            return false;
        }
        auto midRenderer_Ptr = SDL_CreateRenderer(this->window_.get(), "vulkan");
        if (midRenderer_Ptr == nullptr) {
            spdlog::error("[{}]创建 SDL_Renderer 失败: {}", this->Get_ClassName(), SDL_GetError());
            return false;
        }
        this->renderer_ = raii::SDL_RendererPtr(midRenderer_Ptr);
        if (!this->renderer_) {
            spdlog::error("[{}]创建 SDL_RendererPtr 失败", this->Get_ClassName());
            raii::SDL_RendererDelete()(midRenderer_Ptr);
            return false;
        }
        const char* name = SDL_GetRendererName(midRenderer_Ptr);
        spdlog::trace("[{}]Renderer 后端: {}", this->Get_ClassName(), name ? name : "Unknown");
        SDL_SetRenderVSync(this->renderer_.get(), this->config_->Get_VSyncState());
        SDL_SetWindowPosition(this->window_.get(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
        SDL_SetWindowMinimumSize(this->window_.get(), this->config_->Get_WindowSize().x, this->config_->Get_WindowSize().y);
        return true;
    }

    bool Core::Init_Timer() {
        this->timer_ = std::make_unique<datetime::Timer>();
        if (!this->timer_) {
            spdlog::error("[{}]Timer 创建失败", this->Get_ClassName());
            return false;
        }
        return this->timer_->Start(this->config_->Get_Interval());
    }

    bool Core::Init_InputManager() {
        this->inputManager_ = std::make_unique<input::InputManager>();
        if (!this->inputManager_) {
            spdlog::error("[{}]InputManager 创建失败", this->Get_ClassName());
            return false;
        }
        return true;
    }

    bool Core::Init_ResourceManager() {
        this->resourceManager_ = std::make_unique<resource::ResourceManager>();
        if (!this->resourceManager_) {
            spdlog::error("[{}]ResourceManager 创建失败", this->Get_ClassName());
            return false;
        }
        return true;
    }

    bool Core::Init_ImGui() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImPlot::CreateContext();
        ImPlot3D::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        ImGuiStyle& style = ImGui::GetStyle();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
        // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
        //io.ConfigViewportsNoAutoMerge = true;
        //io.ConfigViewportsNoTaskBarIcon = true;
        auto midData = theme::ThemeToStyle(theme::ImGuiTheme_Darcula);
        style = midData;
        style.ScaleAllSizes(this->config_->Get_MainScale());        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
        style.FontScaleDpi = this->config_->Get_MainScale();        // Set initial font scale. (in docking branch: using io.ConfigDpiScaleFonts=true automatically overrides this for every window depending on the current monitor)
        style.DockingSeparatorSize = 1.0f;/* 默认是2像素 */
        io.ConfigDpiScaleFonts = true;          // [Experimental] Automatically overwrite style.FontScaleDpi in Begin() when Monitor DPI changes. This will scale fonts but _NOT_ scale sizes/padding for now.
        io.ConfigDpiScaleViewports = true;      // [Experimental] Scale Dear ImGui and Platform Windows when Monitor DPI changes.

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }
        // Setup Platform/Renderer backends
        ImGui_ImplSDL3_InitForSDLRenderer(this->window_.get(), this->renderer_.get());
        ImGui_ImplSDLRenderer3_Init(this->renderer_.get());
        ImFont* font = io.Fonts->AddFontFromFileTTF("assets/fonts/NotoSansSC-Regular.ttf", 20.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());
        IM_ASSERT(font != nullptr);
        ImFontConfig config;
        config.MergeMode = true;          // 合并到主字体，方便直接用字符串显示图标
        config.PixelSnapH = true;         // 像素对齐，让边缘更清晰
        config.GlyphMinAdvanceX = 32.0f;  // 【关键】设置图标的最小水平宽度，数值可根据你的字体大小调整
        config.GlyphMaxAdvanceX = 32.0f;  // 【关键】设置图标的最大水平宽度，数值可根据你的字体大小调整
        static const ImWchar iconRanges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
        font = io.Fonts->AddFontFromFileTTF("assets/fonts/Font_Awesome_6_Free-Solid-900.otf", 24.0, &config, iconRanges);
        IM_ASSERT(font != nullptr);
        io.IniFilename = "assets/config/ini/main.ini";
        this->imguiInitState_ = true;
        return true;
    }

    bool Core::Init_Context() {
        this->context_ = std::make_unique<Context>(*this->config_, *this->inputManager_, *this->resourceManager_, this->window_.get(), this->trayType_, this->renderer_.get(), this->runningState_, this->appResult_);
        if (!this->context_) {
            spdlog::error("[{}]Context 创建失败", this->Get_ClassName());
            return false;
        }
        if (!this->inputManager_->Init(*this->context_)) { return false; }
        if (!this->resourceManager_->Init(*this->context_)) { return false; }
        return true;
    }

    bool Core::Init_SceneManager() {
        this->sceneManager_ = std::make_unique<scene::SceneManager>(*this->context_);
        if (!this->sceneManager_) {
            spdlog::error("[{}]SceneManager 创建失败", this->Get_ClassName());
            return false;
        }
        auto scene = std::make_unique<app::scene::AppScene>("AppScene", *this->context_, *this->sceneManager_);
        this->sceneManager_->Request_Push_Scene(std::move(scene));
        return true;
    }

    bool Core::Init_Icon() {
        if (this->config_->Get_IconPath().empty()) {
            spdlog::error("[{}]Icon 获取失败: 路径为空", this->Get_ClassName());
            return false;
        }
        this->resourceManager_->Get_SurfaceManager()->Insert(this->config_->Get_IconPath());
        auto midSurace_Ptr = this->resourceManager_->Get_SurfaceManager()->Search(this->config_->Get_IconPath());
        if (!midSurace_Ptr) {
            return false;
        }
        SDL_SetWindowIcon(this->window_.get(), midSurace_Ptr);
        auto midTray_Ptr = SDL_CreateTray(midSurace_Ptr, this->config_->Get_WindowTitle().c_str());
        if (!midTray_Ptr) {
            spdlog::error("[{}]SDL_Tray 创建失败: {}", this->Get_ClassName(), SDL_GetError());
            return false;
        }
        this->trayType_.tray = raii::SDL_TrayPtr(midTray_Ptr);
        if (!this->trayType_.tray) {
            spdlog::error("[{}]SDL_Tray 创建失败", this->Get_ClassName());
            raii::SDL_TrayDelete()(midTray_Ptr);
            return false;
        }
        this->trayType_.menu = SDL_CreateTrayMenu(this->trayType_.tray.get());/* 为托盘创建右键菜单 */
        if (!this->trayType_.menu) {
            spdlog::error("[{}]SDL_TrayMenu 创建失败", this->Get_ClassName());
            return false;
        }

        // 3. 在菜单中添加一个“退出”按钮
        SDL_TrayEntry* entry = SDL_InsertTrayEntryAt(this->trayType_.menu, -1, "退出", SDL_TRAYENTRY_BUTTON);
        this->trayType_.entry.emplace(entry, std::pair<SDL_TrayCallback, void*>(Core::Quit_TryCallback, this));

        // 4. 绑定回调函数
        SDL_SetTrayEntryCallback(entry, Core::Quit_TryCallback, this);
        return true;
    }

    SDL_AppResult Core::Input(SDL_Event& event) {
        ImGui_ImplSDL3_ProcessEvent(&event);
        if (event.type == SDL_EVENT_QUIT)
            this->runningState_ = false;
        if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(this->window_.get()))
            this->runningState_ = false;
        this->inputManager_->Input(event);
        this->sceneManager_->Input(event);
        if (!this->runningState_) { return SDL_APP_SUCCESS; }
        return this->appResult_.load();
    }

    void Core::Update() {
        this->sceneManager_->Update(this->timer_->Get_FrameTime_S_Float());
    }

    void Core::Render() {
        ImGuiIO& io = ImGui::GetIO();
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        this->sceneManager_->Render();

        ImGui::Render();
        SDL_SetRenderScale(this->renderer_.get(), io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        SDL_SetRenderDrawColorFloat(this->renderer_.get(), this->backColor.x, this->backColor.y, this->backColor.z, this->backColor.w);
        SDL_RenderClear(this->renderer_.get());
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), this->renderer_.get());
        SDL_RenderPresent(this->renderer_.get());
        this->timer_->End_FrameTime();
        this->timer_->Set_TimerState(false);
    }

    void Core::Clean() {
        if (this->context_) {
            this->context_.reset();
        }
        if (this->sceneManager_) {
            this->sceneManager_->Clean();
            this->sceneManager_.reset();
        }
        if (this->resourceManager_) {
            this->resourceManager_->Clean();
            this->resourceManager_.reset();
        }
        if (this->inputManager_) {
            this->inputManager_.reset();
        }
        if (this->timer_) {
            this->timer_.reset();
        }
        if (this->imguiInitState_) {
            ImGui_ImplSDLRenderer3_Shutdown();
            ImGui_ImplSDL3_Shutdown();
            ImPlot3D::DestroyContext();
            ImPlot::DestroyContext();
            ImGui::DestroyContext();
            this->imguiInitState_ = false;
            this->renderer_.reset();
            this->window_.reset();
            this->trayType_.tray.reset();
            SDL_Quit();
            spdlog::trace("[{}]资源全部清理完毕", this->Get_ClassName());
        }
        if (this->config_) {
            this->config_.reset();
        }
    }

    bool Core::Get_RunningState()const {
        return this->runningState_;
    }

    SDL_AppResult Core::Get_AppResult()const {
        return this->appResult_.load();
    }

    datetime::Timer& Core::Get_Timer() {
        return *this->timer_.get();
    }

    void Core::Quit_TryCallback(void* userdata, SDL_TrayEntry* entry) {
        if (!userdata || !entry) {
            return;
        }
        auto midCore = static_cast<Core*>(userdata);
        midCore->appResult_.store(SDL_APP_SUCCESS);
        midCore->runningState_ = false;
    }
} // namespace engine::core
