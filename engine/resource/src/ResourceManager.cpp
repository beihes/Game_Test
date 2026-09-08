#include "../inc/ResourceManager.hpp"
#include "../inc/SurfaceManager.hpp"
#include "../inc/TextureManager.hpp"
#include "engine/core/inc/Context.hpp"
#include <spdlog/spdlog.h>

namespace engine::resource {
    ResourceManager::ResourceManager() {
        spdlog::trace("[{}]ResourceManager 构建完成", this->Get_ClassName());
    }
    ResourceManager::~ResourceManager() {
        this->Clean();
        spdlog::trace("[{}]ResourceManager 析构完成", this->Get_ClassName());
    }

    bool ResourceManager::Init(core::Context& context) {
        this->surfaceManager_ = std::make_unique<SurfaceManager>();
        if (!this->surfaceManager_) {
            spdlog::error("[{}]SurfaceManager 创建失败", this->Get_ClassName());
            return false;
        }
        this->context_ = context;
        this->textureManager_ = std::make_unique<TextureManager>(context.Get_Renderer());
        if (!this->textureManager_) {
            spdlog::error("Texture管理器创建失败");
            return false;
        }
        return true;
    }

    void ResourceManager::Clean() {
        if (this->surfaceManager_) {
            this->surfaceManager_->Clean();
            this->surfaceManager_.reset();
        }
        if (this->textureManager_) {
            this->textureManager_->Clean();
            this->textureManager_.reset();
        }
    }

    SurfaceManager* ResourceManager::Get_SurfaceManager() {
        return this->surfaceManager_.get();
    }

    TextureManager* ResourceManager::Get_TextureManager() {
        return this->textureManager_.get();
    }
}