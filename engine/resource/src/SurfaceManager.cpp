#include "../inc/SurfaceManager.hpp"
#include <spdlog/spdlog.h>
#include <optional>

namespace engine::resource {
    SurfaceManager::SurfaceManager() {
        spdlog::trace("[{}]SurfaceManager 构建完成", this->Get_ClassName());
    }

    SurfaceManager::~SurfaceManager() {
        spdlog::trace("[{}]SurfaceManager 析构完成", this->Get_ClassName());
    }

    SDL_Surface* SurfaceManager::Insert(std::string_view surfacePath) {
        if (surfacePath.empty()) { return nullptr; }
        auto midSurface_Ptr = IMG_Load(surfacePath.data());
        if (!midSurface_Ptr) {
            spdlog::warn("[{}]图片 '{}' 插入 Surface 管理器失败", this->Get_ClassName(), surfacePath);
            return nullptr;
        }
        auto midSurfacePtr = sdl::SDL_SurfacePtr(midSurface_Ptr);
        if (!midSurface_Ptr) {
            spdlog::warn("[{}]图片 '{}' 插入 Surface 管理器失败", this->Get_ClassName(), surfacePath);
            SDL_DestroySurface(midSurface_Ptr);
            return nullptr;
        }
        auto [insertIt, result] = this->surfaces_.insert_or_assign(surfacePath.data(), std::move(midSurfacePtr));
        if (!result) {
            spdlog::warn("[{}]图片 '{}' 插入 Surface 管理器失败", this->Get_ClassName(), surfacePath);
            return nullptr;
        }
        spdlog::trace("[{}]图片 '{}' 插入 Surface 管理器成功", this->Get_ClassName(), surfacePath);
        return insertIt->second.get();
    }

    SDL_Surface* SurfaceManager::Insert(std::string_view midName, SDL_Surface* midSurface) {
        if (!midSurface && midName.empty()) { return nullptr; }
        auto midData = this->surfaces_.find(midName.data());
        bool needLock = SDL_MUSTLOCK(midSurface);
        if (midData == this->surfaces_.end() || (midData->second->w != midSurface->w || midData->second->h != midSurface->h)) {
            if (needLock) { SDL_LockSurface(midSurface); }
            auto midSurface_Ptr = SDL_DuplicateSurface(midSurface);
            if (!midSurface_Ptr) {
                spdlog::warn("[{}]图片 '{}' 插入 Surface 管理器失败", this->Get_ClassName(), midName);
                if (needLock) { SDL_UnlockSurface(midSurface); }
                return nullptr;
            }
            if (needLock) { SDL_UnlockSurface(midSurface); }
            auto midSurfacePtr = sdl::SDL_SurfacePtr(midSurface_Ptr);
            if (!midSurfacePtr) {
                spdlog::warn("[{}]图片 '{}' 插入 Surface 管理器失败", this->Get_ClassName(), midName);
                SDL_DestroySurface(midSurface_Ptr);
                return nullptr;
            }
            auto [insertIt, result] = this->surfaces_.insert_or_assign(midName.data(), std::move(midSurfacePtr));
            if (!result) {
                spdlog::warn("[{}]图片 '{}' 插入 Surface 管理器失败", this->Get_ClassName(), midName);
                return nullptr;
            }
            spdlog::trace("[{}]图片 '{}' 插入 Surface 管理器成功", this->Get_ClassName(), midName);
            if (needLock) { SDL_UnlockSurface(midSurface); }
            return insertIt->second.get();
        } else {
            if (!SDL_LockSurface(midData->second.get())) {
                spdlog::error("[{}]锁定纹理失败：{}", this->Get_ClassName(), SDL_GetError());
                return nullptr;
            }
            if (needLock) { SDL_LockSurface(midSurface); }
            if (!SDL_ConvertPixels(midSurface->w, midSurface->h, midSurface->format, midSurface->pixels, midSurface->pitch, midSurface->format, midData->second->pixels, midData->second->pitch)) {
                spdlog::error("[{}]像素转换失败：{}", this->Get_ClassName(), SDL_GetError());
                if (needLock) { SDL_UnlockSurface(midSurface); }
                SDL_UnlockSurface(midData->second.get());
                return nullptr;
            }
            if (needLock) { SDL_UnlockSurface(midSurface); }
            SDL_UnlockSurface(midData->second.get());
        }
        spdlog::trace("[{}]图片 '{}' 插入 Surface 管理器成功", this->Get_ClassName(), midName);
        return midData->second.get();
    }

    SDL_Surface* SurfaceManager::Insert(std::string_view midName, sdl::SDL_SurfacePtr&& midSurfacePtr) {
        if (!midSurfacePtr || midName.empty()) { return nullptr; }
        auto [insertIt, result] = this->surfaces_.insert_or_assign(midName.data(), std::move(midSurfacePtr));
        if (!result) {
            spdlog::warn("[{}]图片 '{}' 插入 Surface 管理器失败", this->Get_ClassName(), midName);
            return nullptr;
        }
        spdlog::trace("[{}]图片 '{}' 插入 Surface 管理器成功", this->Get_ClassName(), midName);
        return insertIt->second.get();
    }

    SDL_Surface* SurfaceManager::Search(std::string_view midName) {
        if (midName.empty()) { return nullptr; }
        auto midData = this->surfaces_.find(midName.data());
        if (midData == this->surfaces_.end()) { return nullptr; }
        return midData->second.get();
    }

    bool SurfaceManager::Delete(std::string_view midName) {
        if (midName.empty()) { return false; };
        auto midData = this->surfaces_.find(midName.data());
        if (midData == this->surfaces_.end()) { return false; }
        this->surfaces_.erase(midData);
        spdlog::trace("[{}]从 Surface 管理器删除图片集 '{}' 成功", this->Get_ClassName(), midName);
        return true;
    }

    void SurfaceManager::Clean() {
        if (this->surfaces_.empty()) { return; }
        for (auto& it : this->surfaces_) {
            spdlog::trace("[{}]准备删除 Surface : '{}'", this->Get_ClassName(), it.first);
        }
        spdlog::trace("[{}]删除 Surface 数量: {}", this->Get_ClassName(), this->surfaces_.size());
        this->surfaces_.clear();
    }
}