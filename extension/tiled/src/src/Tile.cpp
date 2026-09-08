#include "../inc/Tile.hpp"
#include "../inc/Animation.hpp"
#include <spdlog/spdlog.h>

namespace tiled {
    Tile::Tile(int id, const nlohmann::json& jsonData)
        :id_(id), jsonData_(jsonData) {
        spdlog::trace("[{}]Tile 构建完成: {}", this->Get_ClassName(), this->id_);
    }

    Tile::~Tile() {
        spdlog::trace("[{}]Tile 析构完成: {}", this->Get_ClassName(), this->id_);
    }

    bool Tile::Parse() {
        if (this->jsonData_.contains("animation")) {
            auto& midJsonData = this->jsonData_["animation"];
            this->animation_ = std::make_unique<Animation>(midJsonData);
            animation_->Prase();
        }
        if (this->jsonData_.contains("probability")) {
            this->probability_ = this->jsonData_["probability"].get<float>();
        }
        spdlog::trace("[{}]Tile 解析完成: {}", this->Get_ClassName(), this->id_);
        return true;
    }

    void Tile::Print() const {
        spdlog::info("[{}] Tile::Print() id={}, probability={}, hasAnimation={}", this->Get_ClassName(), this->id_, this->probability_, this->animation_ != nullptr);
        if (this->animation_) {
            this->animation_->Print();
        }
    }

    template<typename T>
    bool Tile::Get_JsonValue(const char* name, T& value) const {
        if (!this->jsonData_.contains(name)) {
            spdlog::error("[{}]tiles->{} 缺少字段: {}", this->Get_ClassName(), this->id_, name);
            return false;
        }
        try {
            value = this->jsonData_.at(name).get<T>();
        }
        catch (const nlohmann::json::exception& e) {
            spdlog::error("[{}]tiles->{}->{} 字段类型错误: {}", this->Get_ClassName(), this->id_, name, e.what());
            return false;
        }
        return true;
    }
} // namespace tiled
