#include "../inc/TransformComponent.hpp"
#include <spdlog/spdlog.h>

namespace engine::component {
    TransformComponent::TransformComponent(const glm::vec3& position, const glm::vec3& scale, const glm::quat& roate)
        :position_(position), scale_(scale), rotate_(roate) {
        spdlog::trace("[{}]TransformComponent 构建完成", this->Get_ClassName());
    }

    TransformComponent::TransformComponent(const glm::mat4& matrix) {
        this->position_ = glm::vec3(matrix[3]);
        this->scale_ = std::move(glm::vec3(glm::length(glm::vec3(matrix[0])), glm::length(glm::vec3(matrix[1])), glm::length(glm::vec3(matrix[2]))));
        glm::mat3 rotationMatrix;
        rotationMatrix[0] = glm::vec3(matrix[0]) / this->scale_.x;
        rotationMatrix[1] = glm::vec3(matrix[1]) / this->scale_.y;
        rotationMatrix[2] = glm::vec3(matrix[2]) / this->scale_.z;
        this->rotate_ = std::move(glm::quat_cast(rotationMatrix));
        spdlog::trace("[{}]TransformComponent 构建完成", this->Get_ClassName());
    }

    TransformComponent::~TransformComponent() {
        spdlog::trace("[{}]TransformComponent 析构完成", this->Get_ClassName());
    }

    void TransformComponent::Rotate(float angle, const glm::vec3& axis) {
        this->rotate_ = glm::normalize(glm::angleAxis(angle, axis) * this->rotate_);
    }

    void TransformComponent::Set_Position(const glm::vec3& position) {
        this->position_ = position;
    }
    void TransformComponent::Set_Scale(const glm::vec3& scale) {
        this->scale_ = scale;
    }
    void TransformComponent::Set_Roate(const glm::quat& roate) {
        this->rotate_ = roate;
    }

    glm::mat4 TransformComponent::Get_Matrix() const {
        return glm::translate(glm::mat4(1.0f), this->position_) * glm::mat4_cast(this->rotate_) * glm::scale(glm::mat4(1.0F), this->scale_);
    }
} // namespace engine::component
