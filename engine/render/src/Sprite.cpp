#include "../inc/Sprite.hpp"
#include <spdlog/spdlog.h>

namespace engine::render {
    Sprite::Sprite(const std::string& textureName, const std::optional<glm::vec4>& srcRect, const std::optional<glm::vec4>& dstRect, bool flipState)
        :textureName_(textureName), srcRect_(srcRect), dstRect_(dstRect), flipState_(flipState) {
        if (srcRect.has_value()) {
            spdlog::trace("[{}]Sprite 构建完成: {}, (({},{}),({},{}))", this->Get_ClassName(), textureName, srcRect.value().x, srcRect.value().y, srcRect.value().z, srcRect.value().w);
            return;
        }
        spdlog::trace("[{}]Sprite 构建完成: {}", this->Get_ClassName(), textureName);
    }

    Sprite::~Sprite() {
        if (this->srcRect_.has_value()) {
            spdlog::trace("[{}]Sprite 析构完成: {}, (({},{}),({},{}))", this->Get_ClassName(), this->textureName_, this->srcRect_.value().x, this->srcRect_.value().y, this->srcRect_.value().z, this->srcRect_.value().w);
            return;
        }
        spdlog::trace("[{}]Sprite 析构完成: {}", this->Get_ClassName(), this->textureName_);
    }

    void Sprite::Set_Name(const std::string& textureName) {
        this->textureName_ = textureName;
    }

    void Sprite::Set_SrcRect(const std::optional<glm::vec4>& srcRect) {
        this->srcRect_ = srcRect_;
    }

    void Sprite::Set_DstRect(const std::optional<glm::vec4>& dstRect) {
        this->dstRect_ = dstRect;
    }

    void Sprite::Set_FlipState(bool flipState) {
        this->flipState_ = flipState;
    }

    const std::string& Sprite::Get_Name() const {
        return this->textureName_;
    }

    const std::optional<glm::vec4> Sprite::Get_SrcRect() const {
        return this->srcRect_;
    }

    const bool Sprite::Get_FlipState()const {
        return this->flipState_;
    }
} // namespace engine::render
