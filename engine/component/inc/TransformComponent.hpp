#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#ifdef __cplusplus
}
#endif // __cplusplus

#include "Component.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/matrix.hpp>
#include <glm/ext.hpp>

namespace engine::core {
    class Context;
}

namespace engine::component {
    class TransformComponent final : public component::Component {
        friend class object::Object;//友元不能继承，必须每个子类单独添加
    public:
        explicit TransformComponent(const glm::vec3& position, const glm::vec3& scale, const glm::quat& roate);
        explicit TransformComponent(const glm::mat4& matrix);
        ~TransformComponent() override;
    public:
        //删除拷贝和移动构造函数及赋值运算符
        TransformComponent(const TransformComponent&) = delete;
        TransformComponent& operator=(const TransformComponent&) = delete;
        TransformComponent(TransformComponent&&) = delete;
        TransformComponent& operator=(TransformComponent&&) = delete;
    public:
        void Translate(const glm::vec3& offset) { this->position_ += offset; }
        void Rotate(float angle, const glm::vec3& axis);
    public:
        void Set_Position(const glm::vec3& position);
        void Set_Scale(const glm::vec3& scale);
        void Set_Roate(const glm::quat& roate);
    public:
        glm::mat4 Get_Matrix() const;
        const glm::vec3& Get_Position() const { return this->position_; }
        const glm::vec3& Get_Scale() const { return this->scale_; }
        const glm::quat& Get_Rotation() const { return this->rotate_; }
    private:
        void Update(float, core::Context&)override {}
    private:
        glm::vec3 position_ = glm::vec3(0.f);/* 位置 */
        glm::vec3 scale_ = glm::vec3(1.0f);/* 缩放 */
        glm::quat rotate_ = glm::angleAxis(0.f, glm::vec3(0.0f, 0.0f, 1.0f));/* 旋转 */
    private:
        const char* Get_ClassName() const {
            return typeid(*this).name();
        }
    };
}
