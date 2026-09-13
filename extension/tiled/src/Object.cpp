#include "tiled/Object.hpp"
#include <spdlog/spdlog.h>

namespace tiled {
    Object::Object(const nlohmann::json& jsonData)
        : jsonData_(jsonData) {
        spdlog::trace("[{}]Object 构建完成", this->Get_ClassName());
    }

    Object::~Object() {
        spdlog::trace("[{}]Object 析构完成", this->Get_ClassName());
    }

    bool Object::Parse() {
        if (!this->Get_JsonValue("id", this->id_)) { return false; }
        if (!this->Get_JsonValue("name", this->name_)) { return false; }
        if (!this->Get_JsonValue("type", this->type_)) { return false; }
        if (!this->Get_JsonValue("x", this->x_)) { return false; }
        if (!this->Get_JsonValue("y", this->y_)) { return false; }
        if (!this->Get_JsonValue("width", this->width_)) { return false; }
        if (!this->Get_JsonValue("height", this->height_)) { return false; }
        if (!this->Get_JsonValue("rotation", this->rotation_)) { return false; }
        if (!this->Get_JsonValue("opacity", this->opacity_)) { return false; }
        if (!this->Get_JsonValue("visible", this->visible_)) { return false; }
        spdlog::trace("[{}]Object 解析成功: {} [type={}, x={}, y={}, w={}, h={}]", this->Get_ClassName(), this->name_.empty() ? "<unnamed>" : this->name_, this->type_, this->x_, this->y_, this->width_, this->height_);
        return true;
    }

    void Object::Print() const {
        spdlog::info("[{}] Object::Print() id={}, name={}, type={}, x={}, y={}, width={}, height={}, rotation={}, opacity={}, visible={}",
            this->Get_ClassName(), this->id_, this->name_, this->type_, this->x_, this->y_, this->width_, this->height_, this->rotation_, this->opacity_, this->visible_);
    }

    template<typename T>
    bool Object::Get_JsonValue(const char* name, T& value) const {
        if (!this->jsonData_.contains(name)) {
            spdlog::error("[{}] object 缺少字段: {}", this->Get_ClassName(), name);
            return false;
        }
        try {
            value = this->jsonData_.at(name).get<T>();
        } catch (const nlohmann::json::exception& e) {
            spdlog::error("[{}] object 字段 [{}] 类型错误: {}", this->Get_ClassName(), name, e.what());
            return false;
        }
        return true;
    }

    template bool Object::Get_JsonValue<int>(const char*, int&) const;
    template bool Object::Get_JsonValue<double>(const char*, double&) const;
    template bool Object::Get_JsonValue<std::string>(const char*, std::string&) const;
    template bool Object::Get_JsonValue<bool>(const char*, bool&) const;
} // namespace tiled
