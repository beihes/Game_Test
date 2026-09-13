#include "tiled/tiled.hpp"
#include <spdlog/spdlog.h>
#include <fstream>
#include <string>

namespace tiled {
    bool Load_JsonData(const std::string& source, nlohmann::json& jsonData) {
        std::ifstream file(source);
        if (!file.is_open()) {
            spdlog::warn("[{}]无法打开文件: {}", __func__, source);
            return false;
        }
        auto midJsonData = nlohmann::json::parse(file, nullptr, false);
        if (midJsonData.is_discarded()) {
            spdlog::warn("[{}]json 数据解析失败: {}", __func__, source);
            return false;
        }
        jsonData = std::move(midJsonData);
        return true;
    }
} // namespace tiled
