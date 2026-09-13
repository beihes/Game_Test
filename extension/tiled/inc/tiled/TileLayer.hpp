#pragma once
#include "Layer.hpp"
#include <nlohmann/json.hpp>
#include <typeinfo>

namespace tiled {
    class TileLayer : public Layer {
    public:
        TileLayer(const nlohmann::json& jsonData);
        ~TileLayer();
    public:
        bool Parse() override;
        void Print() override;
    public:
        const std::string& Get_Compression()const { return this->compression_; }
        const std::string& Get_Encoding()const { return this->encoding_; }
        int Get_Height()const { return this->height_; }
        int Get_Width()const { return this->width_; }
        bool Get_Visible() const { return Layer::Get_Visible(); }
        const std::vector<uint32_t>& Get_Data() const { return this->data_; }
    private:
        bool Decode_Data(const std::string& data);
        bool Decode_Data_Array(const nlohmann::json& dataArray);
    private:
        std::string compression_ = "";
        std::string encoding_ = "";
        int height_ = 0;
        int width_ = 0;
    private:
        std::vector<uint32_t> data_;
    private:
        const char* Get_ClassName() const {
            return typeid(*this).name();
        }
    };

} // namespace tiled
