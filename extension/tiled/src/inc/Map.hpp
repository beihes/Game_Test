#pragma once
#include <nlohmann/json.hpp>
#include <memory>
#include <string>
#include <typeinfo>
#include <unordered_map>

namespace tiled {
    class Layer;
    class TileSet;
    class Map {
    public:
        Map(std::string_view directory, std::string_view name);
        ~Map();
    public:
        bool Parse();
        void Load();
        void Print() const;
    public:
        const std::string& Get_Name()const;
        const std::string& Get_Orientation()const;
        const std::string& Get_RenderOrder()const;
        const std::string& Get_Type()const;
        int Get_CompressionLevel()const;
        int Get_Height()const;
        int Get_Width()const;
        int Get_TileHeight()const;
        int Get_TileWidth()const;
        int Get_NextLayerId()const;
        int Get_NextObjectId()const;
        bool Get_Infinite()const;
        const std::unordered_map<std::string, std::unique_ptr<Layer>>& Get_Layers() const { return this->layers_; }
        const std::unordered_map<std::string, std::unique_ptr<TileSet>>& Get_TileSets() const { return this->tileSets_; }
        const std::string& Get_Directory() const { return this->directory_; }
        const std::string& Get_Path() const { return this->path_; }
    private:
        template<typename T>
        bool Get_JsonValue(const char* name, T& value) const;
    private:
        std::unordered_map<std::string, std::unique_ptr<Layer>> layers_;
        std::unordered_map<std::string, std::unique_ptr<TileSet>> tileSets_;
    private:
        std::string directory_ = "";
        std::string path_ = "";
        std::string name_ = "";
        std::string orientation_ = "";
        std::string renderOrder_ = "";
        std::string type_ = "";
        int compressionLevel_ = 0;
        int height_ = 0;
        int width_ = 0;
        int tileHeight_ = 0;
        int tileWidth_ = 0;
        int nextLayerId_ = 0;
        int nextObjectId_ = 0;
        bool infinite_ = false;
    private:
        nlohmann::json jsonData_;
    private:
        const char* Get_ClassName() const {
            return typeid(*this).name();
        }
    };
} // namespace tiled
