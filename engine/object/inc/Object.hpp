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

#include <string>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <type_traits>
#include <utility>

#include "component/inc/Component.hpp"

namespace engine::core {
    class Context;
}

namespace engine::object {
    class Object final {
    public:
        Object() = delete;
        Object(std::string_view name = "", std::string_view tag = "");
        virtual ~Object();// 虚函数确保正确清理派生类
    public:
        //删除拷贝和移动构造函数及赋值运算符
        Object(const Object&) = delete;
        Object& operator=(const Object&) = delete;
        Object(Object&&) = delete;
        Object& operator=(Object&&) = delete;
    public:
        template <typename T, typename... Args>
        T* Add_Component(Args&&...args);
        template <typename T>
        T* Get_Component() const;
        template <typename T>
        bool Has_Component()const;
        template <typename T>
        void Remove_Component();
    public:
        void Update(float deltaTime, core::Context& context);
        void Input(core::Context& context);
        void Render(core::Context& context);
        void Clean();
    public:
        void Set_Name(const std::string& name) { this->name_ = name; }
        void Set_Tag(const std::string& tag) { this->tag_ = tag; }
        void Set_NeedRemoveState(bool needRemove) { this->needRemoveState_ = needRemove; }
    public:
        const std::string& Get_Name()const { return this->name_; }
        const std::string& Get_Tag()const { return this->tag_; }
        bool Get_NeedRemoveState()const { return this->needRemoveState_; }
    private:
        std::string name_;
        std::string tag_;
        std::unordered_map<std::type_index, std::unique_ptr<component::Component>> components_;
        bool needRemoveState_ = false;//延迟删除的标志，将来由场景类负责删除
    private:
        const char* GetClassName() const {
            return typeid(*this).name();
        }
    };
}
