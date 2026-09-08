#include "../inc/Map.hpp"
#include "../inc/ObjectGroup.hpp"
#include "../inc/TileLayer.hpp"
#include "../inc/TileSet.hpp"
#include "tiled/tiled.hpp"
#include <spdlog/spdlog.h>
#include <fstream>

namespace tiled {
    Map::Map(std::string_view directory, std::string_view name)
        :directory_(directory), name_(name) {
        this->path_ = this->directory_ + "/" + this->name_;
        spdlog::trace("[{}]Map 构建完成: {}", this->Get_ClassName(), this->path_);
    }

    Map::~Map() {
        spdlog::trace("[{}]Map 析构完成: {}", this->Get_ClassName(), this->path_);
    }

    bool Map::Parse() {
        if (!Load_JsonData(this->path_, this->jsonData_)) {
            return false;
        }
        if (!this->Get_JsonValue("compressionlevel", this->compressionLevel_)) { return false; }
        if (!this->Get_JsonValue("height", this->height_)) { return false; }
        if (!this->Get_JsonValue("width", this->width_)) { return false; }
        if (!this->Get_JsonValue("tileheight", this->tileHeight_)) { return false; }
        if (!this->Get_JsonValue("tilewidth", this->tileWidth_)) { return false; }
        if (!this->Get_JsonValue("orientation", this->orientation_)) { return false; }
        if (!this->Get_JsonValue("renderorder", this->renderOrder_)) { return false; }
        if (!this->Get_JsonValue("type", this->type_)) { return false; }
        if (!this->Get_JsonValue("nextlayerid", this->nextLayerId_)) { return false; }
        if (!this->Get_JsonValue("nextobjectid", this->nextObjectId_)) { return false; }
        if (!this->Get_JsonValue("infinite", this->infinite_)) { return false; }
        if (!this->jsonData_.contains("layers")) {
            spdlog::error("[{}] 地图缺少字段: layers", this->Get_ClassName());
            return false;
        }
        for (const auto& layerJson : this->jsonData_["layers"]) {
            if (!layerJson.contains("type")) {
                spdlog::error("[{}] 图层缺少字段: type", this->Get_ClassName());
                return false;
            }
            auto midType = layerJson["type"].get<std::string>();
            if (midType == "tilelayer") {
                auto layer = std::make_unique<TileLayer>(layerJson);
                if (!layer->Parse()) {
                    spdlog::error("[{}] TileLayer 解析失败", this->Get_ClassName());
                    return false;
                }
                auto [it, inserted] = this->layers_.emplace(layer->Get_Name(), std::move(layer));
                if (!inserted) {
                    spdlog::error("[{}] TileLayer 名称重复: {}", this->Get_ClassName(), it->first);
                    return false;
                }
            } else if (midType == "objectgroup") {
                auto layer = std::make_unique<ObjectGroup>(layerJson);
                if (!layer->Parse()) {
                    spdlog::error("[{}] ObjectGroup 解析失败", this->Get_ClassName());
                    return false;
                }
                auto [it, inserted] = this->layers_.emplace(layer->Get_Name(), std::move(layer));
                if (!inserted) {
                    spdlog::error("[{}] ObjectGroup 名称重复: {}", this->Get_ClassName(), it->first);
                    return false;
                }
            } else {
                spdlog::warn("[{}] 未知图层类型: {}", this->Get_ClassName(), midType);
            }
        }
        if (!this->jsonData_.contains("tilesets")) {
            spdlog::error("[{}] {} 地图缺少字段: tilesets", this->Get_ClassName(), this->path_);
            return false;
        }
        for (const auto& tilesetJson : this->jsonData_["tilesets"]) {
            if (!tilesetJson.contains("firstgid") || !tilesetJson.contains("source")) {
                spdlog::error("[{}]{}->tilesets 缺少字段: firstgid 或 source", this->Get_ClassName(), this->path_);
                return false;
            }
            int firstgid = tilesetJson["firstgid"].get<int>();
            std::string source = tilesetJson["source"].get<std::string>();
            std::string midPath = this->directory_ + "/" + source;
            auto midTileSet = std::make_unique<TileSet>(firstgid, midPath);
            midTileSet->Parse();
            auto [it, inserted] = this->tileSets_.emplace(midPath, std::move(midTileSet));
            if (!inserted) {
                spdlog::error("[{}] TileSet 名称重复: {}", this->Get_ClassName(), it->first);
                return false;
            }
        }

        spdlog::trace("[{}] {} 地图解析完成", this->Get_ClassName(), this->path_);
        return true;
    }

    void Load() {

    }

    void Map::Print() const {
        spdlog::info("[{}] Map::Print() name={}, path={}, size={}x{}, tileSize={}x{}, layerCount={}, tilesetCount={}",
            this->Get_ClassName(), this->name_, this->path_, this->width_, this->height_, this->tileWidth_, this->tileHeight_, this->layers_.size(), this->tileSets_.size());
        for (const auto& [layerName, layer] : this->layers_) {
            spdlog::info("[{}]layer={}", this->Get_ClassName(), layerName);
            if (layer) {
                auto* tileLayer = dynamic_cast<TileLayer*>(layer.get());
                if (tileLayer) {
                    tileLayer->Print();
                    continue;
                }
                auto* objectGroup = dynamic_cast<ObjectGroup*>(layer.get());
                if (objectGroup) {
                    objectGroup->Print();
                }
            }
        }
        for (const auto& [tilesetPath, tileset] : this->tileSets_) {
            spdlog::info("[{}]tileset={}", this->Get_ClassName(), tilesetPath);
            if (tileset) { tileset->Print(); }
        }
    }

    const std::string& Map::Get_Name() const {
        return this->name_;
    }

    const std::string& Map::Get_Orientation() const {
        return this->orientation_;
    }

    const std::string& Map::Get_RenderOrder() const {
        return this->renderOrder_;
    }

    const std::string& Map::Get_Type() const {
        return this->type_;
    }

    int Map::Get_CompressionLevel() const {
        return this->compressionLevel_;
    }

    int Map::Get_Height() const {
        return this->height_;
    }

    int Map::Get_Width() const {
        return this->width_;
    }

    int Map::Get_TileHeight() const {
        return this->tileHeight_;
    }

    int Map::Get_TileWidth() const {
        return this->tileWidth_;
    }

    int Map::Get_NextLayerId() const {
        return this->nextLayerId_;
    }

    int Map::Get_NextObjectId() const {
        return this->nextObjectId_;
    }

    bool Map::Get_Infinite() const {
        return this->infinite_;
    }

    template<typename T>
    bool Map::Get_JsonValue(const char* name, T& value) const {
        if (!this->jsonData_.contains(name)) {
            spdlog::error("[{}] 地图缺少字段: {}", this->Get_ClassName(), name);
            return false;
        }
        try {
            value = this->jsonData_.at(name).get<T>();
        } catch (const nlohmann::json::exception& e) {
            spdlog::error("[{}] 地图字段 [{}] 类型错误: {}", this->Get_ClassName(), name, e.what());
            return false;
        }
        return true;
    }
} // namespace tiled
