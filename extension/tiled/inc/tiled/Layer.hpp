#pragma once
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <typeinfo>

namespace tiled {
    class Layer {
    public:
        Layer(const nlohmann::json& jsonData);
        virtual ~Layer();
    public:
        virtual bool Parse() = 0;
        virtual void Print() = 0;
    public:
        const std::string& Get_Name() const { return name_; }
        int Get_Id() const { return id_; }
        int Get_Opacity() const { return opacity_; }
        const std::string& Get_Type() const { return type_; }
        int Get_X()const { return this->x_; }
        int Get_Y()const { return this->y_; }
        bool Get_Visible() const { return this->visible_; }
    protected:
        std::string name_ = "";
        int id_ = 0;
        int opacity_ = 0;
        std::string type_ = "";
        int x_ = 0;
        int y_ = 0;
        bool visible_ = false;
    protected:
        template<typename T>
        bool Get_JsonValue(const char* name, T& value) const;
    protected:
        const nlohmann::json& jsonData_;
    private:
        const char* Get_ClassName() const {
            return typeid(*this).name();
        }
    };
} // namespace tiled
