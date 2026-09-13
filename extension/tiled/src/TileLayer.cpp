#include "tiled/TileLayer.hpp"
#include <fmt/ranges.h>
#include <libbase64.h>
#include <spdlog/spdlog.h>
#include <vector>
#include <zlib.h>

namespace tiled {
    TileLayer::TileLayer(const nlohmann::json& jsonData)
        : Layer(jsonData) {
        spdlog::trace("[{}]TileLayer 构建完成", this->Get_ClassName());
    }

    TileLayer::~TileLayer() {
        spdlog::trace("[{}]TileLayer 析构完成", this->Get_ClassName());
    }

    bool TileLayer::Parse() {
        if (!this->Get_JsonValue("name", this->name_)) { return false; }
        if (!this->Get_JsonValue("type", this->type_)) { return false; }
        if (!this->Get_JsonValue("height", this->height_)) { return false; }
        if (!this->Get_JsonValue("width", this->width_)) { return false; }
        if (!this->Get_JsonValue("x", this->x_)) { return false; }
        if (!this->Get_JsonValue("y", this->y_)) { return false; }
        if (!this->Get_JsonValue("id", this->id_)) { return false; }
        if (!this->Get_JsonValue("opacity", this->opacity_)) { return false; }
        if (!this->Get_JsonValue("visible", this->visible_)) { return false; }
        if (!this->jsonData_.contains("data")) {
            spdlog::error("[{}] TileLayer 缺少字段: data", this->Get_ClassName());
            return false;
        }
        const auto& dataJson = this->jsonData_["data"];
        if (dataJson.is_string()) {
            if (this->jsonData_.contains("compression")) {
                this->compression_ = this->jsonData_["compression"].get<std::string>();
            }
            if (this->jsonData_.contains("encoding")) {
                this->encoding_ = this->jsonData_["encoding"].get<std::string>();
            }
            std::string midData = dataJson.get<std::string>();
            if (!this->Decode_Data(midData)) {
                return false;
            }
        } else if (dataJson.is_array()) {
            if (!this->Decode_Data_Array(dataJson)) {
                return false;
            }
        } else {
            spdlog::error("[{}] TileLayer data 字段类型错误，必须为 string 或 array", this->Get_ClassName());
            return false;
        }
        spdlog::trace("[{}]TileLayer 解析成功: {}", this->Get_ClassName(), this->name_);
        return true;
    }

    void TileLayer::Print() {
        spdlog::info("[{}] TileLayer::Print() name={}, type={}, size={}x{}, pos={}x{}, tileCount={}, compression={}, encoding={}",
            this->Get_ClassName(), this->name_, this->type_, this->width_, this->height_, this->x_, this->y_, this->data_.size(), this->compression_, this->encoding_);
    }

    bool TileLayer::Decode_Data_Array(const nlohmann::json& dataArray) {
        const size_t tileCount = static_cast<size_t>(this->width_) * static_cast<size_t>(this->height_);
        this->data_.clear();
        this->data_.reserve(tileCount);
        for (const auto& value : dataArray) {
            if (!value.is_number_unsigned() && !value.is_number_integer()) {
                spdlog::error("[{}] TileLayer data 数组中存在非整数值", this->Get_ClassName());
                return false;
            }
            this->data_.push_back(static_cast<uint32_t>(value.get<uint32_t>()));
        }
        if (this->data_.size() != tileCount) {
            spdlog::error("[{}] TileLayer data 数量不一致: {} != {}", this->Get_ClassName(), this->data_.size(), tileCount);
            return false;
        }
        return true;
    }

    bool TileLayer::Decode_Data(const std::string& data) {
        std::vector<uint8_t> compressed(data.size() * 3 / 4 + 3);
        size_t compressedSize = compressed.size();
        int result = base64_decode(data.data(), data.size(), reinterpret_cast<char*>(compressed.data()), &compressedSize, 0);
        if (!result) {
            spdlog::error("[{}] {} 的 base64 解码错误: {}", this->Get_ClassName(), this->name_, result);
            return false;
        }
        const size_t tileCount = static_cast<size_t>(this->width_) * static_cast<size_t>(this->height_);
        const size_t expectedSize = tileCount * sizeof(uint32_t);
        std::vector<uint8_t> decompressed(expectedSize);
        uLongf decompressedSize = static_cast<uLongf>(expectedSize);
        result = uncompress(decompressed.data(), &decompressedSize, compressed.data(), static_cast<uLong>(compressed.size()));
        if (result != Z_OK) {
            spdlog::error("[{}] {} 的 zlib 解码错误: {}", this->Get_ClassName(), this->name_, result);
            return false;
        }
        if (decompressedSize != expectedSize) {
            spdlog::error("Invalid Tiled layer size: {} != {}", decompressedSize, expectedSize);
            return false;
        }
        this->data_.resize(tileCount);
        std::memcpy(this->data_.data(), decompressed.data(), expectedSize);
        return true;
    }
} // namespace tiled
