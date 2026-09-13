#include "../inc/TileLayer.hpp"
#include "component/inc/TransformComponent.hpp"
#include "core/inc/Context.hpp"
#include <tiled/tiled.hpp>
#include <spdlog/spdlog.h>

namespace engine::component::tiled {
    TileLayer::TileLayer(const entt::hashed_string& name)
        : name_(name) {
        spdlog::trace("[{}]TileLayer 构建完成", this->Get_ClassName());
    }

    TileLayer::~TileLayer() {
        spdlog::trace("[{}]TileLayer 析构完成", this->Get_ClassName());
    }

    void TileLayer::Init() {
        // this->transformComponent_ = this->owner_ ? this->owner_->Get_Component<TransformComponent>() : nullptr;
    }

    void TileLayer::Update([[maybe_unused]] float deltaTime, [[maybe_unused]] core::Context& context) {

    }

    void TileLayer::Render(core::Context& context) {
        const float scaleX = this->transformComponent_ ? this->transformComponent_->Get_Scale().x : 1.0f;
        const float scaleY = this->transformComponent_ ? this->transformComponent_->Get_Scale().y : 1.0f;
        const float positionX = this->transformComponent_ ? this->transformComponent_->Get_Position().x : 0.0f;
        const float positionY = this->transformComponent_ ? this->transformComponent_->Get_Position().y : 0.0f;
    }

    void TileLayer::Clean() {
        this->transformComponent_ = nullptr;
        this->texture_ = nullptr;
        this->tileSize_ = glm::vec2(0.0f);
        this->mapSize_ = glm::vec2(0.0f);
        this->visibleState_ = false;
    }

    void TileLayer::Set_VisibleState(bool visibleState) {
        this->visibleState_ = visibleState;
    }
} // namespace engine::component
