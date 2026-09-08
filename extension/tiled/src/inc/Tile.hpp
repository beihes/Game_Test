#pragma once
#include <nlohmann/json.hpp>
#include <memory>
#include <string>
#include <typeinfo>
#include <unordered_map>

namespace tiled {
    class Animation;
    class Tile {
    public:
        Tile(int id, const nlohmann::json& jsonData);
        ~Tile();
    public:
        bool Parse();
        void Print() const;
    public:
        int Get_Id()const { return this->id_; }
        float Get_Probability()const { return this->probability_; }
        const Animation* Get_Animation()const { return this->animation_.get(); }
    private:
        template<typename T>
        bool Get_JsonValue(const char* name, T& value) const;
    private:
        int id_ = 0;
        float probability_ = 1.0;
        std::unique_ptr<Animation> animation_;
    private:
        const nlohmann::json& jsonData_;
    private:
        const char* Get_ClassName() const {
            return typeid(*this).name();
        }
    };

} // namespace tiled
