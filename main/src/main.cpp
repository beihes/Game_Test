#include "../inc/main.hpp"
#include "app/scene/inc/AppScene.hpp"
#include "engine/core/inc/Config.hpp"
#include "engine/core/inc/Context.hpp"
#include "engine/core/inc/Core.hpp"
#include "engine/input/inc/InputManager.hpp"
#include "engine/resource/inc/ResourceManager.hpp"
#include "engine/scene/inc/SceneManager.hpp"
#include <datetime/datetime.hpp>
#include <spdlog/spdlog.h>

static engine::core::Core core;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void** appstate, [[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    spdlog::set_level(spdlog::level::trace);
    if (!core.Init()) {
        return SDL_APP_FAILURE;
    }
    *appstate = &core;
    spdlog::trace("[{}]<----应用开启---->", __func__);
    return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent([[maybe_unused]] void* appstate, SDL_Event* event) {
    return core.Input(*event); /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate([[maybe_unused]] void* appstate) {
    core.Get_Timer().Start_FrameTime();
    if (core.Get_Timer().Get_TimerState()) {
        core.Update();
        core.Render();
        core.Get_Timer().End_FrameTime();
    }
    return core.Get_AppResult(); /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit([[maybe_unused]] void* appstate, SDL_AppResult result) {
    /* SDL will clean up the window/renderer for us. */
    if (result != SDL_APP_SUCCESS) {
        spdlog::error("[{}]应用出现错误, 强制关闭中", __func__);
    }
    spdlog::trace("[{}]>----应用关闭----<", __func__);
    core.Clean();
}
