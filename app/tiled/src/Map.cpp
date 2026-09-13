#include "../inc/Map.hpp"
#include "../inc/tilelayer.hpp"
#include "scene/inc/AppContext.hpp"
#include "scene/inc/AppScene.hpp"
#include "engine/component/tiled/inc/Layer.hpp"
#include "engine/component/tiled/inc/TileLayer.hpp"
#include "engine/core/inc/Context.hpp"
#include "engine/render/inc/Sprite.hpp"
#include "engine/resource/inc/TextureManager.hpp"
#include "engine/resource/inc/ResourceManager.hpp"
#include "engine/scene/inc/Scene.hpp"
#include "engine/scene/inc/SceneManager.hpp"
#include <spdlog/spdlog.h>
#include <entt/core/hashed_string.hpp>

using namespace entt::literals;

namespace app::tiled {
    ::tiled::TileSet* Search_TileSet_From_Gid(::tiled::Map* map, uint32_t gid) {
        for (auto& it : map->Get_TileSets()) {
            if (gid >= it.second->Get_FirstGid() && gid < (it.second->Get_EndGid())) {
                return it.second.get();
            }
        }
        return nullptr;
    }

    std::optional<engine::render::Sprite> Search_Sprite_From_TileSet(uint32_t gid, ::tiled::TileSet* tileSet) {
        if (!tileSet || gid < tileSet->Get_FirstGid() || gid >= tileSet->Get_EndGid() ||
            tileSet->Get_Columns() <= 0 || tileSet->Get_TileWidth() <= 0 || tileSet->Get_TileHeight() <= 0) {
            return std::nullopt;
        }
        const int localId = gid - tileSet->Get_FirstGid();
        const int column = localId % tileSet->Get_Columns();
        const int row = localId / tileSet->Get_Columns();
        const glm::vec4 sourceRect(
            static_cast<float>(tileSet->Get_Margin() + column * (tileSet->Get_TileWidth() + tileSet->Get_Spacing())),
            static_cast<float>(tileSet->Get_Margin() + row * (tileSet->Get_TileHeight() + tileSet->Get_Spacing())),
            static_cast<float>(tileSet->Get_TileWidth()),
            static_cast<float>(tileSet->Get_TileHeight()));
        return std::move(engine::render::Sprite(tileSet->Get_ImagePath(), sourceRect));
    }

    std::optional<engine::render::Sprite> Search_Sprite_From_Gid(::tiled::Map* map, uint32_t gid) {
        constexpr uint32_t FLIP_MASK = 0xF0000000;
        uint32_t cleanGid = static_cast<uint32_t>(gid) & ~FLIP_MASK;
        ::tiled::TileSet* tileSet = nullptr;
        for (auto& it : map->Get_TileSets()) {
            if (cleanGid >= it.second->Get_FirstGid() && cleanGid < (it.second->Get_EndGid())) {
                tileSet = it.second.get();
                break;
            }
        }
        if (!tileSet) { return std::nullopt; }
        const uint32_t localId = cleanGid - tileSet->Get_FirstGid();
        const int column = localId % tileSet->Get_Columns();
        const int row = localId / tileSet->Get_Columns();
        const glm::vec4 sourceRect(
            static_cast<float>(tileSet->Get_Margin() + column * (tileSet->Get_TileWidth() + tileSet->Get_Spacing())),
            static_cast<float>(tileSet->Get_Margin() + row * (tileSet->Get_TileHeight() + tileSet->Get_Spacing())),
            static_cast<float>(tileSet->Get_TileWidth()),
            static_cast<float>(tileSet->Get_TileHeight()));
        return engine::render::Sprite(tileSet->Get_ImagePath(), sourceRect);
    }

    Map::Map() {
        spdlog::trace("[{}]Map 构建完成", this->Get_ClassName());
    }

    Map::~Map() {
        spdlog::trace("[{}]Map 析构完成", this->Get_ClassName());
    }

    void Map::Init(const std::string& directory, const std::string& name, scene::AppContext& appContext) {
        this->appContext_ = appContext;
        this->map_ = std::make_unique<::tiled::Map>(directory, name);
        this->map_->Parse();
        for (auto& it : this->map_->Get_TileSets()) {
            if (it.second) {
                appContext.Get_Context().Get_ResourceManager().Get_TextureManager()->Insert(it.second->Get_ImagePath());
            }
        }
        glm::ivec2 tileSize = glm::ivec2(this->map_->Get_TileWidth(), this->map_->Get_TileHeight());
        for (auto& it : this->map_->Get_Layers()) {
            if (it.second) {
                auto* midData = dynamic_cast<::tiled::TileLayer*>(it.second.get());
                if (midData) {
                    auto entity = appContext.appScene_.registry_.create();
                    glm::ivec2 mapSize = glm::ivec2(midData->Get_Width(), midData->Get_Height());
                    std::vector<engine::render::Sprite> spriteVector;
                    spriteVector.reserve(midData->Get_Data().size());
                    for (int i = 0;i < midData->Get_Data().size();i++) {
                        auto sprite = Search_Sprite_From_Gid(this->map_.get(), midData->Get_Data()[i]);
                        if (!sprite.has_value()) {
                            continue;
                        }
                        if (sprite->srcRect_.has_value()) {
                            sprite->dstRect_ = glm::vec4(
                                static_cast<float>((i % midData->Get_Width()) * tileSize.x),
                                static_cast<float>((i / midData->Get_Width()) * tileSize.y),
                                sprite->srcRect_->z,
                                sprite->srcRect_->w);
                        }
                        spriteVector.push_back(std::move(*sprite));
                    }
                    auto& tileLayer = appContext.appScene_.registry_.emplace<engine::component::tiled::Layer>(entity, midData->Get_Id(), mapSize, tileSize, spriteVector, &(this->appContext_->get()));
                    tileLayer.renderCallback = tilelayer::Render_Callback;
                }
            }
        }
    }

    void Map::Input(const SDL_Event& event) {

    }

    void Map::Update(float deltaTime) {

    }

    void Map::Render() {

    }

    void Map::Clear() {

    }
} // namespace app::tiled
