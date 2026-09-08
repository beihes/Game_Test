#include "../inc/TileSet.hpp"
#include "../inc/Tile.hpp"
#include "tiled/tiled.hpp"
#include <spdlog/spdlog.h>
#include <fstream>

namespace tiled {
    TileSet::TileSet(int firstgid, std::string_view source)
        :firstgid_(firstgid), source_(source) {
        spdlog::trace("[{}]TileSet 构建完成: {}", this->Get_ClassName(), this->source_);
    }

    TileSet::~TileSet() {
        spdlog::trace("[{}]TileSet 析构完成: {}", this->Get_ClassName(), this->source_);
    }

    bool TileSet::Parse() {
        if (!Load_JsonData(this->source_, this->jsonData_)) {
            return false;
        }
        if (!this->Get_JsonValue("columns", this->columns_)) { return false; }
        if (!this->Get_JsonValue("image", this->image_)) { return false; }
        if (!this->Get_JsonValue("imageheight", this->imageHeight_)) { return false; }
        if (!this->Get_JsonValue("imagewidth", this->imageWidth_)) { return false; }
        if (!this->Get_JsonValue("margin", this->margin_)) { return false; }
        if (!this->Get_JsonValue("name", this->name_)) { return false; }
        if (!this->Get_JsonValue("spacing", this->spacing_)) { return false; }
        if (!this->Get_JsonValue("tilecount", this->tilecount_)) { return false; }
        if (!this->Get_JsonValue("tileheight", this->tileHeight_)) { return false; }
        if (!this->Get_JsonValue("tilewidth", this->tileWidth_)) { return false; }
        if (!this->Get_JsonValue("type", this->type_)) { return false; }
        if (this->jsonData_.contains("tiles")) {
            for (const auto& tileJson : this->jsonData_["tiles"]) {
                if (!tileJson.contains("id")) {
                    spdlog::error("[{}] TileSet 缺少字段: id", this->Get_ClassName());
                    return false;
                }
                int id = tileJson.at("id").get<int>();
                auto tile = std::make_unique<Tile>(id, tileJson);
                if (!tile->Parse()) {
                    spdlog::error("[{}] Tile 解析失败: {}", this->Get_ClassName(), id);
                    return false;
                }
                auto [it, inserted] = this->tiles_.emplace(id, std::move(tile));
                if (!inserted) {
                    spdlog::error("[{}] Tile ID 重复: {}", this->Get_ClassName(), it->first);
                    return false;
                }
            }
        }
        spdlog::trace("[{}] {} TileSet 解析完成", this->Get_ClassName(), this->source_);
        return true;
    }

    void TileSet::Print() const {
        spdlog::info("[{}] TileSet::Print() source={}, name={}, columns={}, tileCount={}, tileSize={}x{}, image={} ",
            this->Get_ClassName(), this->source_, this->name_, this->columns_, this->tilecount_, this->tileWidth_, this->tileHeight_, this->image_);
        for (const auto& [tileId, tile] : this->tiles_) {
            spdlog::info("[{}]tileId={} ", this->Get_ClassName(), tileId);
            tile->Print();
        }
    }

    template<typename T>
    bool TileSet::Get_JsonValue(const char* name, T& value) const {
        if (!this->jsonData_.contains(name)) {
            spdlog::error("[{}]{} 缺少字段: {}", this->Get_ClassName(), this->source_, name);
            return false;
        }
        try {
            value = this->jsonData_.at(name).get<T>();
        }
        catch (const nlohmann::json::exception& e) {
            spdlog::error("[{}]{} 字段 [{}] 类型错误: {}", this->Get_ClassName(), this->source_, name, e.what());
            return false;
        }
        return true;
    }
} // namespace tiled
