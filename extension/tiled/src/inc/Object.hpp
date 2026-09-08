#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <typeinfo>

namespace tiled {
    class Object {
    public:
        Object(const nlohmann::json& jsonData);
        ~Object();
    public:
        bool Parse();
        void Print() const;
    public:
        const std::string& Get_Name() const { return this->name_; }
        const std::string& Get_Type() const { return this->type_; }
        int Get_Id() const { return this->id_; }
        double Get_X() const { return this->x_; }
        double Get_Y() const { return this->y_; }
        double Get_Width() const { return this->width_; }
        double Get_Height() const { return this->height_; }
        double Get_Rotation() const { return this->rotation_; }
        double Get_Opacity() const { return this->opacity_; }
        bool Get_Visible() const { return this->visible_; }
    private:
        template<typename T>
        bool Get_JsonValue(const char* name, T& value) const;
    private:
        const nlohmann::json& jsonData_;
        std::string name_ = "";
        std::string type_ = "";
        int id_ = 0;
        double x_ = 0.0;
        double y_ = 0.0;
        double width_ = 0.0;
        double height_ = 0.0;
        double rotation_ = 0.0;
        double opacity_ = 1.0;
        bool visible_ = true;
    private:
        const char* Get_ClassName() const {
            return typeid(*this).name();
        }
    };
} // namespace tiled
