#include "tiled/Layer.hpp"
#include <spdlog/spdlog.h>

namespace tiled {
    Layer::Layer(const nlohmann::json& jsonData)
        : jsonData_(jsonData) {
        spdlog::trace("[{}]Layer 构建完成", this->Get_ClassName());
    }

    Layer::~Layer() {
        spdlog::trace("[{}]Layer 析构完成", this->Get_ClassName());
    }

    template<typename T>
    bool Layer::Get_JsonValue(const char* name, T& value) const {
        if (!this->jsonData_.contains(name)) {
            spdlog::error("[{}] layer 缺少字段: {}", this->Get_ClassName(), name);
            return false;
        }
        try {
            value = this->jsonData_.at(name).get<T>();
        } catch (const nlohmann::json::exception& e) {
            spdlog::error("[{}] layer 字段 [{}] 类型错误: {}", this->Get_ClassName(), name, e.what());
            return false;
        }
        return true;
    }
    /* 放在 .cpp 里必须要显示实例化 */
    template bool Layer::Get_JsonValue<std::string>(const char*, std::string&) const;
    template bool Layer::Get_JsonValue<int>(const char*, int&) const;
    template bool Layer::Get_JsonValue<bool>(const char*, bool&) const;
} // namespace tiled
