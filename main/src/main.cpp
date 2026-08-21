#include "../inc/main.hpp"
#include <spdlog/spdlog.h>

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
    spdlog::set_level(spdlog::level::trace);
    spdlog::trace("[{}]<----应用开启---->", __func__);
    return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
    return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void* appstate) {
    return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    /* SDL will clean up the window/renderer for us. */
    if (result != SDL_APP_SUCCESS) {
        spdlog::error("应用出现错误, 强制关闭中");
    }
    spdlog::debug("[{}]>----应用关闭----<", __func__);
}
