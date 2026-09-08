#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <typeinfo>
#include <vector>

namespace tiled {
    class Animation {
    public:
        struct Frame {
            Frame(int duration, int tileId) :duration_(duration), tileId_(tileId) {}
            int duration_ = 0;
            int tileId_ = 0;
        };
    public:
        Animation(const nlohmann::json& jsonData);
        ~Animation();
    public:
        bool Prase();
        void Print() const;
    public:
        const std::vector<Frame>& Get_Frames()const;
    private:
        std::vector<Frame> frames_;
    private:
        const nlohmann::json& jsonData_;
    private:
        const char* Get_ClassName() const {
            return typeid(*this).name();
        }
    };

} // namespace tiled