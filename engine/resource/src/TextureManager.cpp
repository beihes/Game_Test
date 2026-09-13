#include "../inc/TextureManager.hpp"
#include <spdlog/spdlog.h>

namespace engine::resource {
    TextureManager::TextureManager(SDL_Renderer* renderer)
        :renderer_(renderer) {
        spdlog::trace("[{}]TextureManager 构建完成", this->Get_ClassName());
    }

    TextureManager::~TextureManager() {
        spdlog::trace("[{}]TextureManager 析构完成", this->Get_ClassName());
    }

    SDL_Texture* TextureManager::Insert(std::string_view texturePath) {
        if (texturePath.empty()) { return nullptr; }
        auto midTexture_1Ptr = IMG_LoadTexture(this->renderer_, texturePath.data());
        if (!midTexture_1Ptr) {
            spdlog::warn("[{}]图片 '{}' 插入 Texture 管理器失败", this->Get_ClassName(), texturePath);
            return nullptr;
        }
        auto midTexturePtr = raii::SDL_TexturePtr(midTexture_1Ptr);
        if (!midTexture_1Ptr) {
            spdlog::warn("[{}]图片 '{}' 插入 Texture 管理器失败", this->Get_ClassName(), texturePath);
            SDL_DestroyTexture(midTexture_1Ptr);
            return nullptr;
        }
        auto [insertIt, result] = this->textures_.insert_or_assign(texturePath.data(), std::move(midTexturePtr));
        if (!result) {
            spdlog::warn("[{}]图片 '{}' 插入 Texture 管理器失败", this->Get_ClassName(), texturePath);
            return nullptr;
        }
        spdlog::trace("[{}]图片 '{}' 插入 Texture 管理器成功", this->Get_ClassName(), texturePath);
        return insertIt->second.get();
    }

    SDL_Texture* TextureManager::Insert(std::string_view midName, SDL_Surface* midSurface_Ptr) {
        if (!midSurface_Ptr || midName.empty()) {
            spdlog::error("[{}]插入 '{}' texture失败, Surface 为空或者midNum为空", this->Get_ClassName(), midName);
            return nullptr;
        }
        SDL_Texture* midTexture = nullptr;
        auto midData = this->textures_.find(midName.data());
        bool needLock = SDL_MUSTLOCK(midSurface_Ptr);
        if (midData == this->textures_.end() || (midData->second->w != midSurface_Ptr->w || midData->second->h != midSurface_Ptr->h)) {
            if (needLock) { SDL_LockSurface(midSurface_Ptr); }
            auto midTexture_Ptr = SDL_CreateTexture(this->renderer_, midSurface_Ptr->format, SDL_TEXTUREACCESS_STREAMING, midSurface_Ptr->w, midSurface_Ptr->h);
            if (needLock) { SDL_UnlockSurface(midSurface_Ptr); }
            if (!midTexture_Ptr) {
                spdlog::error("[{}]插入 '{}' texture 失败: {}", this->Get_ClassName(), midName, SDL_GetError());
                return nullptr;
            }
            auto midTexturePtr = raii::SDL_TexturePtr(midTexture_Ptr);
            if (!midTexturePtr) {
                spdlog::error("[{}]插入 '{}' texture 失败", this->Get_ClassName(), midName);
                SDL_DestroyTexture(midTexture_Ptr);
                return nullptr;
            }
            auto [insertIt, result] = this->textures_.insert_or_assign(std::string(midName), std::move(midTexturePtr));
            if (!result) {
                spdlog::error("[{}]插入 '{}' texture 失败", this->Get_ClassName(), midName);
                return nullptr;
            }
            midTexture = insertIt->second.get();
        } else {
            midTexture = midData->second.get();
        }
        void* pixels;
        int pitch;
        if (!SDL_LockTexture(midTexture, nullptr, &pixels, &pitch)) {
            spdlog::error("[{}]锁定纹理失败：{}", this->Get_ClassName(), SDL_GetError());
            return nullptr;
        }
        if (needLock) { SDL_LockSurface(midSurface_Ptr); }
        if (!SDL_ConvertPixels(midSurface_Ptr->w, midSurface_Ptr->h, midSurface_Ptr->format, midSurface_Ptr->pixels, midSurface_Ptr->pitch, midSurface_Ptr->format, pixels, pitch)) {
            spdlog::error("[{}]像素转换失败：{}", this->Get_ClassName(), SDL_GetError());
            if (needLock) { SDL_UnlockSurface(midSurface_Ptr); }
            SDL_UnlockTexture(midTexture);
            return nullptr;
        }
        if (needLock) { SDL_UnlockSurface(midSurface_Ptr); }
        SDL_UnlockTexture(midTexture);
        spdlog::trace("[{}]图片 '{}' 插入 Texture 管理器成功", this->Get_ClassName(), midName);
        return midTexture;
    }

    SDL_Texture* TextureManager::Insert(std::string_view midName, SDL_Texture* midTexture) {
        if (!midTexture || midName.empty()) {
            spdlog::error("[{}]插入 '{}' texture 失败, Texture 为空或者 midNum 为空", this->Get_ClassName(), midName);
            return nullptr;
        }
        auto midData = this->textures_.find(midName.data());
        auto midTexturePtr = raii::SDL_TexturePtr(midTexture);
        if (!midTexturePtr) {
            spdlog::error("[{}]插入 '{}' texture失败时创建失败", this->Get_ClassName(), midName);
            return nullptr;
        }
        auto [insertIt, result] = this->textures_.insert_or_assign(std::string(midName), std::move(midTexturePtr));
        if (!result) {
            spdlog::error("[{}]插入 '{}' texture失败", this->Get_ClassName(), midName);
            return nullptr;
        }
        spdlog::trace("[{}]图片 '{}' 插入 Texture 管理器成功", this->Get_ClassName(), midName);
        return insertIt->second.get();
    }

    SDL_Texture* TextureManager::Insert(std::string_view midName, raii::SDL_TexturePtr&& midTexturePtr) {
        if (!midTexturePtr || midName.empty()) {
            spdlog::error("[{}]插入 '{}' texture 失败, Texture 为空或者 midNum 为空", this->Get_ClassName(), midName);
            return nullptr;
        }
        auto [insertIt, result] = this->textures_.insert_or_assign(std::string(midName), std::move(midTexturePtr));
        if (!result) {
            spdlog::error("[{}]插入 '{}' texture失败", this->Get_ClassName(), midName);
            return nullptr;
        }
        spdlog::trace("[{}]图片 '{}' 插入 Texture 管理器成功", this->Get_ClassName(), midName);
        return insertIt->second.get();
    }

    SDL_Texture* TextureManager::Search(std::string_view midName) {
        auto it = this->textures_.find(midName.data());
        if (it == this->textures_.end()) {
            return nullptr;
        }
        return it->second.get();
    }

    bool TextureManager::Delete(std::string_view midName) {
        auto it = this->textures_.find(midName.data());
        if (it == this->textures_.end()) {
            return false;
        }
        this->textures_.erase(it);
        spdlog::trace("[{}]从 Texture 管理器删除图片 '{}' 成功", this->Get_ClassName(), midName);
        return true;
    }

    void TextureManager::Clean() {
        if (this->textures_.empty())return;
        for (auto& it : this->textures_) {
            spdlog::trace("[{}]准备删除 Texture : '{}'", this->Get_ClassName(), it.first);
        }
        spdlog::trace("[{}]删除 Texture : {}", this->Get_ClassName(), this->textures_.size());
        this->textures_.clear();
    }
}