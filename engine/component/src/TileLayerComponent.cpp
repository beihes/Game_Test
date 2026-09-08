#include "../inc/TileLayerComponent.hpp"
#include "../inc/TransformComponent.hpp"
#include "object/inc/Object.hpp"
#include "core/inc/Context.hpp"
#include <tiled/tiled.hpp>
#include <spdlog/spdlog.h>

namespace engine::component {
    TileLayerComponent::TileLayerComponent() {
        spdlog::trace("[{}]TileLayerComponent 构建完成", this->Get_ClassName());
    }

    TileLayerComponent::~TileLayerComponent() {
        spdlog::trace("[{}]TileLayerComponent 析构完成", this->Get_ClassName());
    }

    void TileLayerComponent::Init() {
        this->transformComponent_ = this->owner_ ? this->owner_->Get_Component<TransformComponent>() : nullptr;
    }

    void TileLayerComponent::Update([[maybe_unused]] float deltaTime, [[maybe_unused]] core::Context& context) {
        if (!this->layer_) {
            this->visible_ = false;
            return;
        }
        this->visible_ = this->enabled_ && this->layer_->Get_Visible();
    }

    void TileLayerComponent::Render(core::Context& context) {
        if (!this->visible_ || !this->layer_ || !this->tileSet_ || !this->texture_) {
            return;
        }

        SDL_Renderer* renderer = context.Get_Renderer();
        if (!renderer || this->tileSet_->Get_Columns() <= 0 || this->tileSet_->Get_TileWidth() <= 0 || this->tileSet_->Get_TileHeight() <= 0) {
            return;
        }

        const float scaleX = this->transformComponent_ ? this->transformComponent_->Get_Scale().x : 1.0f;
        const float scaleY = this->transformComponent_ ? this->transformComponent_->Get_Scale().y : 1.0f;
        const float positionX = this->transformComponent_ ? this->transformComponent_->Get_Position().x : 0.0f;
        const float positionY = this->transformComponent_ ? this->transformComponent_->Get_Position().y : 0.0f;
        const int columns = this->tileSet_->Get_Columns();
        const int tileWidth = this->tileSet_->Get_TileWidth();
        const int tileHeight = this->tileSet_->Get_TileHeight();
        const int margin = this->tileSet_->Get_Margin();
        const int spacing = this->tileSet_->Get_Spacing();
        const uint32_t firstGid = static_cast<uint32_t>(this->tileSet_->Get_FirstGid());
        const uint32_t tileCount = static_cast<uint32_t>(this->tileSet_->Get_TileCount());
        const auto& data = this->layer_->Get_Data();
        const int layerWidth = this->layer_->Get_Width();

        for (size_t index = 0; index < data.size(); ++index) {
            const uint32_t gid = data[index] & 0x1FFFFFFFu;
            if (gid < firstGid || (tileCount > 0 && gid >= firstGid + tileCount) || layerWidth <= 0) {
                continue;
            }

            const uint32_t localId = gid - firstGid;
            const int sourceColumn = static_cast<int>(localId % static_cast<uint32_t>(columns));
            const int sourceRow = static_cast<int>(localId / static_cast<uint32_t>(columns));
            const int destinationColumn = static_cast<int>(index % static_cast<size_t>(layerWidth));
            const int destinationRow = static_cast<int>(index / static_cast<size_t>(layerWidth));
            SDL_FRect source{
                static_cast<float>(margin + sourceColumn * (tileWidth + spacing)),
                static_cast<float>(margin + sourceRow * (tileHeight + spacing)),
                static_cast<float>(tileWidth),
                static_cast<float>(tileHeight)
            };
            SDL_FRect destination{
                positionX + static_cast<float>(destinationColumn * tileWidth) * scaleX,
                positionY + static_cast<float>(destinationRow * tileHeight) * scaleY,
                static_cast<float>(tileWidth) * scaleX,
                static_cast<float>(tileHeight) * scaleY
            };
            SDL_RenderTexture(renderer, this->texture_, &source, &destination);
        }
    }

    void TileLayerComponent::Clean() {
        this->transformComponent_ = nullptr;
        this->layer_ = nullptr;
        this->tileSet_ = nullptr;
        this->texture_ = nullptr;
        this->tileSize_ = glm::vec2(0.0f);
        this->mapSize_ = glm::vec2(0.0f);
        this->enabled_ = true;
        this->visible_ = false;
    }

    void TileLayerComponent::Set_Layer(const tiled::TileLayer* layer) {
        this->layer_ = layer;
        if (layer) {
            this->mapSize_ = glm::vec2(static_cast<float>(layer->Get_Width()), static_cast<float>(layer->Get_Height()));
            this->visible_ = this->enabled_ && layer->Get_Visible();
        } else {
            this->mapSize_ = glm::vec2(0.0f);
            this->visible_ = false;
        }
    }

    void TileLayerComponent::Set_TileSet(const tiled::TileSet* tileSet) {
        this->tileSet_ = tileSet;
        this->tileSize_ = tileSet ? glm::vec2(static_cast<float>(tileSet->Get_TileWidth()), static_cast<float>(tileSet->Get_TileHeight())) : glm::vec2(0.0f);
    }

    void TileLayerComponent::Set_Texture(SDL_Texture* texture) {
        this->texture_ = texture;
    }

    void TileLayerComponent::Set_Visible(bool visible) {
        this->enabled_ = visible;
        this->visible_ = visible && this->layer_ && this->layer_->Get_Visible();
    }
} // namespace engine::component
