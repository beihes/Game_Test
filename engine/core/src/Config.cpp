#include "../inc/Config.hpp"
extern "C" {
#include <libavutil/avutil.h>
#include <libavutil/version.h>
#include <libavcodec/version.h>
#include <libavformat/version.h>
#include <libswscale/version.h>
#include <libswresample/version.h>
}

#include <glm/glm.hpp>
#include <imgui.h>
#include <implot.h>
#include <implot3d.h>
#include <nlohmann/json.hpp>
#include <SDL3_image/SDL_image.h>
#include <spdlog/spdlog.h>
#include <spdlog/version.h>
#include <unicode/uvernum.h>

namespace engine::core {
    void Config::Print_All_Lib_Version() {
        spdlog::trace("[{}]spdlog 版本: {}.{}.{}", this->Get_ClassName(), SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR, SPDLOG_VER_PATCH);
        spdlog::trace("[{}]SDL 版本: {}.{}.{}, 链接版本: {}", this->Get_ClassName(), SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_MICRO_VERSION, SDL_GetRevision());
        spdlog::trace("[{}]SDL_image 版本: {}.{}.{}", this->Get_ClassName(), SDL_IMAGE_MAJOR_VERSION, SDL_IMAGE_MINOR_VERSION, SDL_IMAGE_MICRO_VERSION);
        spdlog::trace("[{}]SDL_mixer 版本: {}.{}.{}", this->Get_ClassName(), SDL_MIXER_MAJOR_VERSION, SDL_MIXER_MINOR_VERSION, SDL_MIXER_MICRO_VERSION);
        spdlog::trace("[{}]SDL_net 版本: {}.{}.{}", this->Get_ClassName(), SDL_NET_MAJOR_VERSION, SDL_NET_MINOR_VERSION, SDL_NET_MICRO_VERSION);
        spdlog::trace("[{}]SDL_ttf 版本: {}.{}.{}", this->Get_ClassName(), SDL_TTF_MAJOR_VERSION, SDL_TTF_MINOR_VERSION, SDL_TTF_MICRO_VERSION);
        spdlog::trace("[{}]libavutil 版本: {}.{}.{}", this->Get_ClassName(), LIBAVUTIL_VERSION_MAJOR, LIBAVUTIL_VERSION_MINOR, LIBAVUTIL_VERSION_MICRO);
        spdlog::trace("[{}]libavcodec 版本: {}.{}.{}", this->Get_ClassName(), LIBAVCODEC_VERSION_MAJOR, LIBAVCODEC_VERSION_MINOR, LIBAVCODEC_VERSION_MICRO);
        spdlog::trace("[{}]libavformat 版本: {}.{}.{}", this->Get_ClassName(), LIBAVFORMAT_VERSION_MAJOR, LIBAVFORMAT_VERSION_MINOR, LIBAVFORMAT_VERSION_MICRO);
        spdlog::trace("[{}]libswscale 版本: {}.{}.{}", this->Get_ClassName(), LIBSWSCALE_VERSION_MAJOR, LIBSWSCALE_VERSION_MINOR, LIBSWSCALE_VERSION_MICRO);
        spdlog::trace("[{}]libswresample 版本: {}.{}.{}", this->Get_ClassName(), LIBSWRESAMPLE_VERSION_MAJOR, LIBSWRESAMPLE_VERSION_MINOR, LIBSWRESAMPLE_VERSION_MICRO);
        spdlog::trace("[{}]FFmpeg 版本: {}", this->Get_ClassName(), av_version_info());
        spdlog::trace("[{}]imgui 版本: {}", this->Get_ClassName(), IMGUI_VERSION);
        spdlog::trace("[{}]implot 版本: {}", this->Get_ClassName(), IMPLOT_VERSION);
        spdlog::trace("[{}]implot3d 版本: {}", this->Get_ClassName(), IMPLOT3D_VERSION);
        spdlog::trace("[{}]glm 版本: {}.{}.{}", this->Get_ClassName(), GLM_VERSION_MAJOR, GLM_VERSION_MINOR, GLM_VERSION_PATCH);
        spdlog::trace("[{}]nlohmann_json 版本: {}.{}.{}", this->Get_ClassName(), NLOHMANN_JSON_VERSION_MAJOR, NLOHMANN_JSON_VERSION_MINOR, NLOHMANN_JSON_VERSION_PATCH);
        spdlog::trace("[{}]ICU 版本: {}.{}.{}", this->Get_ClassName(), U_ICU_VERSION_MAJOR_NUM, U_ICU_VERSION_MINOR_NUM, U_ICU_VERSION_PATCHLEVEL_NUM);
    }

    void Config::Set_InitFlags(SDL_InitFlags midInitFlags) {
        this->initFlags = midInitFlags;
    }

    void Config::Set_WindowFlags(SDL_WindowFlags midWindowFlags) {
        this->windowFlags = midWindowFlags;
    }

    void Config::Set_MainScale(float midMainScale) {
        this->mainScale = midMainScale;
    }

    void Config::Set_WindowTitle(std::string_view midWindowTitle) {
        this->windowTitle = midWindowTitle;
    }

    void Config::Set_WindowSize(SDL_Point midFRect) {
        this->windowSize = midFRect;
    }

    void Config::Set_VSyncState(int midVSyncState) {
        this->vsyncState = midVSyncState;
    }

    SDL_InitFlags Config::Get_InitFlags()const {
        return this->initFlags;
    }

    SDL_WindowFlags Config::Get_WindowFlags()const {
        return this->windowFlags;
    }

    float Config::Get_MainScale() const {
        return this->mainScale;
    }

    std::string Config::Get_WindowTitle() const {
        return this->windowTitle;
    }
    SDL_Point Config::Get_WindowSize() const {
        return this->windowSize;
    }
    int Config::Get_VSyncState() const {
        return this->vsyncState;
    }

    Uint32 Config::Get_Interval()const {
        return this->interval;
    }

    const char* Config::Get_IniFilePath()const {
        return this->iniFilePath;
    }

    const std::string& Config::Get_IconPath()const {
        return this->iconPath_;
    }

    const std::string& Config::Get_DatabaseContents()const {
        return this->databaseContents_;
    }

    const std::string& Config::Get_ImageContents()const {
        return this->imageContents_;
    }
    const std::string& Config::Get_VideoContents()const {
        return this->videoContents_;
    }

    const std::unordered_map<std::string, std::vector<std::string>>& Config::Get_KeyMap()const {
        return this->keyMap_;
    }
}