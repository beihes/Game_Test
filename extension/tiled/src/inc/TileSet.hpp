#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <typeinfo>
#include <unordered_map>

namespace tiled {
    class Tile;
    class TileSet {
    public:
        TileSet(int firstgid, std::string_view source);
        ~TileSet();
    public:
        bool Parse();
        void Print() const;
    public:
        int Get_FirstGid() const { return this->firstgid_; }
        const std::string& Get_Name() const { return this->name_; }
        const std::string& Get_Image() const { return this->image_; }
        const std::string& Get_Type() const { return this->type_; }
        int Get_Columns() const { return this->columns_; }
        int Get_ImageHeight() const { return this->imageHeight_; }
        int Get_ImageWidth() const { return this->imageWidth_; }
        int Get_Margin() const { return this->margin_; }
        int Get_Spacing() const { return this->spacing_; }
        int Get_TileCount() const { return this->tilecount_; }
        int Get_TileHeight() const { return this->tileHeight_; }
        int Get_TileWidth() const { return this->tileWidth_; }
        const std::unordered_map<int, std::unique_ptr<Tile>>& Get_Tiles() const { return this->tiles_; }
    private:
        template<typename T>
        bool Get_JsonValue(const char* name, T& value) const;
    private:
        std::unordered_map<int, std::unique_ptr<Tile>> tiles_;
    private:
        std::string name_ = "";
        std::string image_ = "";
        std::string type_ = "";
        int columns_ = 0;
        int imageHeight_ = 0;
        int imageWidth_ = 0;
        int margin_ = 0;
        int spacing_ = 0;
        int tilecount_ = 0;
        int tileHeight_ = 0;
        int tileWidth_ = 0;
    private:
        nlohmann::json jsonData_;
    private:
        int firstgid_ = 0;
        std::string source_ = "";
    private:
        const char* Get_ClassName() const {
            return typeid(*this).name();
        }
    };
} // namespace tiled
