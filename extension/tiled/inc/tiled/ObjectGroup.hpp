#pragma once
#include "Layer.hpp"
#include "Object.hpp"
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <typeinfo>
#include <vector>

namespace tiled {
    class ObjectGroup :public Layer {
    public:
        ObjectGroup(const nlohmann::json& jsonData);
        ~ObjectGroup()override;
    public:
        bool Parse()override;
        void Print()override;
    public:
        const std::string& Get_Draworder()const { return this->draworder_; }
    public:
        const std::vector<std::unique_ptr<Object>>& Get_Objects() const { return this->objects_; }
    private:
        std::string draworder_ = "";
    private:
        std::vector<std::unique_ptr<Object>> objects_;
    private:
        const char* Get_ClassName() const {
            return typeid(*this).name();
        }
    };



} // namespace tiled
