#include "../inc/Component.hpp"
#include <spdlog/spdlog.h>

namespace engine::component {
    Component::Component() {
        spdlog::trace("[{}]Component 构建完成", this->Get_ClassName());
    }

    Component::~Component() {
        spdlog::trace("[{}]Component 析构完成", this->Get_ClassName());
    }
} // namespace engine::component
