#include "tiled/Animation.hpp"
#include <spdlog/spdlog.h>

namespace tiled {
    Animation::Animation(const nlohmann::json& jsonData)
        :jsonData_(jsonData) {
        spdlog::trace("[{}]Animation 构建完成", this->Get_ClassName());
    }

    Animation::~Animation() {
        spdlog::trace("[{}]Animation 析构完成", this->Get_ClassName());
    }

    bool Animation::Prase() {
        for (auto& frameJson : this->jsonData_) {
            if (!frameJson.contains("duration") || !frameJson.contains("tileid")) {
                spdlog::trace("[{}]Animation 缺少字段: duration 或 tileid", this->Get_ClassName());
                return false;
            }
            int duration = frameJson["duration"].get<int>();
            int tileid = frameJson["tileid"].get<int>();
            this->frames_.push_back(Animation::Frame(duration, tileid));
        }
        spdlog::trace("[{}]Animation 解析完成: {}", this->Get_ClassName(), this->frames_[0].tileId_);
        return true;
    }

    void Animation::Print() const {
        spdlog::info("[{}] frame count={} ", this->Get_ClassName(), this->frames_.size());
        for (const auto& frame : this->frames_) {
            spdlog::info("  - duration={}, tileId={}", frame.duration_, frame.tileId_);
        }
    }

    const std::vector<Animation::Frame>& Animation::Get_Frames()const {
        return this->frames_;
    }
} // namespace tiled