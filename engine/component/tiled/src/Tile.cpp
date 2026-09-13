#include "../inc/Tile.hpp"
#include <spdlog/spdlog.h>

namespace engine::component::tiled {
    Tile::Tile(/* args */) {
        spdlog::trace("[{}]Tile 构建完成", this->Get_ClassName());
    }

    Tile::~Tile() {
        spdlog::trace("[{}]Tile 析构完成", this->Get_ClassName());
    }

    void Tile::Init() {
        // this->transformComponent_ = this->owner_ ? this->owner_->Get_Component<TransformComponent>() : nullptr;
    }

    void Tile::Update([[maybe_unused]] float deltaTime, [[maybe_unused]] core::Context& context) {

    }

    void Tile::Render(core::Context& context) {

    }

    void Tile::Clean() {
        this->visibleState_ = false;
    }
} // namespace engine::component::tiled
