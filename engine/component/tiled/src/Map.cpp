#include "../inc/Map.hpp"
#include <spdlog/spdlog.h>

namespace engine::component::tiled {
    Map::Map(const entt::hashed_string& name)
        : name_(name) {
        spdlog::trace("[{}]Map 构建完成", this->Get_ClassName());
    }

    Map::~Map() {
        spdlog::trace("[{}]Map 析构完成", this->Get_ClassName());
    }

    void Map::Init() {
        // this->transformComponent_ = this->owner_ ? this->owner_->Get_Component<TransformComponent>() : nullptr;
    }

    void Map::Update([[maybe_unused]] float deltaTime, [[maybe_unused]] core::Context& context) {

    }

    void Map::Render(core::Context& context) {

    }

    void Map::Clean() {
        this->visibleState_ = false;
    }
} // namespace engine::component::tiled
