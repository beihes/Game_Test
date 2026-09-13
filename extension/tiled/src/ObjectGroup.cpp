#include "tiled/ObjectGroup.hpp"
#include <spdlog/spdlog.h>

namespace tiled {
    ObjectGroup::ObjectGroup(const nlohmann::json& jsonData)
        : Layer(jsonData) {
        spdlog::trace("[{}]ObjectGroup 构建完成", this->Get_ClassName());
    }

    ObjectGroup::~ObjectGroup() {
        spdlog::trace("[{}]ObjectGroup 析构完成", this->Get_ClassName());
    }

    bool ObjectGroup::Parse() {
        if (!this->Get_JsonValue("draworder", this->draworder_)) { return false; }
        if (!this->Get_JsonValue("name", this->name_)) { return false; }
        if (!this->Get_JsonValue("id", this->id_)) { return false; }
        if (!this->Get_JsonValue("type", this->type_)) { return false; }
        if (!this->Get_JsonValue("opacity", this->opacity_)) { return false; }
        if (!this->Get_JsonValue("visible", this->visible_)) { return false; }
        if (!this->Get_JsonValue("x", this->x_)) { return false; }
        if (!this->Get_JsonValue("y", this->y_)) { return false; }
        if (!this->jsonData_.contains("objects")) {
            spdlog::error("[{}] 对象分组缺少字段: objects", this->Get_ClassName());
            return false;
        }
        for (const auto& objectJson : this->jsonData_["objects"]) {
            auto object = std::make_unique<Object>(objectJson);
            if (!object->Parse()) {
                spdlog::error("[{}] 对象解析失败: {}", this->Get_ClassName(), this->name_);
                return false;
            }
            this->objects_.push_back(std::move(object));
        }

        spdlog::trace("[{}]ObjectGroup 解析成功: {} (objects={})", this->Get_ClassName(), this->name_, this->objects_.size());
        return true;
    }

    void ObjectGroup::Print() {
        spdlog::info("[{}] ObjectGroup::Print() name={}, type={}, draworder={}, objectCount={}",
            this->Get_ClassName(), this->name_, this->type_, this->draworder_, this->objects_.size());
        for (const auto& object : this->objects_) {
            if (object) {
                object->Print();
            }
        }
    }
} // namespace tiled
