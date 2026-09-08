#include "../inc/Object.hpp"
#include "component/inc/Component.hpp"
#include "core/inc/Context.hpp"
#include <spdlog/spdlog.h>

namespace engine::object {
    Object::Object(std::string_view name, std::string_view tag)
        :name_(name), tag_(tag) {
        spdlog::trace("[{}]Object 构建完成: {} {}", this->GetClassName(), this->name_, this->tag_);
    }

    Object::~Object() {
        spdlog::trace("[{}]Object 析构完成: {} {}", this->GetClassName(), this->name_, this->tag_);
    }

    template <typename T, typename... Args>
    T* Object::Add_Component(Args&&...args) {
        static_assert(std::is_base_of<component::Component, T>::value, "T 必须继承自 Component");
        auto typeIndex = std::type_index(typeid(T));
        if (this->Has_Component<T>()) {
            return this->Get_Component<T>();
        }
        auto newComponent = std::make_unique<T>(std::forward<Args>(args)...);
        T* ptr = newComponent.get();
        newComponent->Set_Owner(this);
        this->components_[typeIndex] = std::move(newComponent);
        ptr->Init();
        return ptr;
    }

    template <typename T>
    T* Object::Get_Component() const {
        static_assert(std::is_base_of<component::Component, T>::value, "T 必须继承自 Component");
        auto it = this->components_.find(std::type_index(typeid(T)));
        return it == this->components_.end() ? nullptr : static_cast<T*>(it->second.get());
    }

    template <typename T>
    bool Object::Has_Component() const {
        static_assert(std::is_base_of<component::Component, T>::value, "T 必须继承自 Component");
        return this->components_.find(std::type_index(typeid(T))) != this->components_.end();
    }

    template <typename T>
    void Object::Remove_Component() {
        static_assert(std::is_base_of<component::Component, T>::value, "T 必须继承自 Component");
        auto it = this->components_.find(std::type_index(typeid(T)));
        if (it == this->components_.end()) {
            return;
        }
        it->second->Clean();
        this->components_.erase(it);
    }

    void Object::Update(float deltaTime, core::Context& context) {
        for (auto& pair : this->components_) {
            pair.second->Update(deltaTime, context);
        }
    }

    void Object::Input(core::Context& context) {
        for (auto& pair : this->components_) {
            pair.second->Input(context);
        }
    }

    void Object::Render(core::Context& context) {
        for (auto& pair : this->components_) {
            pair.second->Render(context);
        }
    }

    void Object::Clean() {
        for (auto& pair : this->components_) {
            pair.second->Clean();
        }
    }
} // namespace engine::object
