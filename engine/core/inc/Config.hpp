#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include <SDL3/SDL.h>
    //#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_net/SDL_net.h>
#include <SDL3_ttf/SDL_ttf.h>
#ifdef __cplusplus
}
#endif // __cplusplus

#include <string>
#include <typeinfo>
#include <unordered_map>
#include <vector>

namespace engine::core {
    class Config final {
    public:
        Config() = default;
        ~Config() = default;
    public:
        void Print_All_Lib_Version();
    public:
        void Set_InitFlags(SDL_InitFlags midInitFlags);
        void Set_WindowFlags(SDL_WindowFlags midWindowFlags);
        void Set_MainScale(float midMainScale);
        void Set_WindowTitle(std::string_view midWindowTitle);
        void Set_WindowSize(SDL_Point midFRect);
        void Set_VSyncState(int midVSyncState);
    public:
        SDL_InitFlags Get_InitFlags()const;
        SDL_WindowFlags Get_WindowFlags()const;
        float Get_MainScale()const;
        std::string Get_WindowTitle()const;
        SDL_Point Get_WindowSize()const;
        int Get_VSyncState()const;
        Uint32 Get_Interval()const;
        const char* Get_IniFilePath()const;
        const std::string& Get_IconPath()const;
        const std::string& Get_DatabaseContents()const;
        const std::string& Get_ImageContents()const;
        const std::string& Get_VideoContents()const;
        const std::unordered_map<std::string, std::vector<std::string>>& Get_KeyMap()const;
    public:
        const char* logFilePath_ = "assets/logs/main.log";
        const char* loggerName_ = "main";
    private:
        /* 窗口配置方面 */
        SDL_InitFlags initFlags = SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_CAMERA;
        SDL_WindowFlags windowFlags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_VULKAN /* | SDL_WINDOW_MAXIMIZED */ /* | SDL_WINDOW_OPENGL */ /* | SDL_WINDOW_BORDERLESS */;
        float mainScale = 1.0;
        std::string windowTitle = "Game-Test";
        SDL_Point windowSize = { 1280,800 };
        int vsyncState = 0;
        Uint32 interval = 10;/* 目标帧时间ms */
        /* imgui配置 */
        const char* iniFilePath = "assets/config/imgui.ini";//imgui的配置文件路径
        std::string assetsPath_ = "assets";
        std::string iconPath_ = "assets/app_settings/icon64x64.ico";
        std::string databaseContents_ = "assets/database/";
        std::string imageContents_ = "assets/image";
        std::string videoContents_ = "assets/video";
    private:
        std::unordered_map<std::string, std::vector<std::string>> keyMap_ = {};
    private:
        const char* Get_ClassName() const {
            return typeid(*this).name();
        }
    };
}