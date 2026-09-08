#include <datetime/datetime.hpp>
#include <tiled/tiled.hpp>
#include <libbase64.h>
#include <SDL3/SDL.h>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include <zlib.h>
#include <fstream>


std::vector<uint32_t> Decode_Tiled_Data(const nlohmann::json& layer) {
    const int width = layer["width"].get<int>();
    const int height = layer["height"].get<int>();
    const std::string data = layer["data"].get<std::string>();
    std::vector<uint8_t> compressed(data.size() * 3 / 4 + 3);
    size_t compressedSize = compressed.size();
    int result = base64_decode(data.data(), data.size(), reinterpret_cast<char*>(compressed.data()), &compressedSize, 0);
    if (result != 0) {
        return {};
    }
    const size_t tileCount = static_cast<size_t>(width) * static_cast<size_t>(height);
    const size_t expectedSize = tileCount * sizeof(uint32_t);
    std::vector<uint8_t> decompressed(expectedSize);
    uLongf decompressedSize = static_cast<uLongf>(expectedSize);
    result = uncompress(decompressed.data(), &decompressedSize, compressed.data(), static_cast<uLong>(compressed.size()));
    if (result != Z_OK) {
        spdlog::error("Failed to decompress Tiled layer: {}", result);
        return {};
    }
    if (decompressedSize != expectedSize) {
        spdlog::error("Invalid Tiled layer size: {} != {}", decompressedSize, expectedSize);
        return {};
    }
    std::vector<uint32_t> gids(tileCount);
    std::memcpy(gids.data(), decompressed.data(), expectedSize);
    return gids;
}

int main() {
    SDL_Init(SDL_INIT_EVENTS);
    spdlog::set_level(spdlog::level::trace);
    tiled::Map midMap("tiled/maps", "Home.tmj");
    midMap.Parse();
    midMap.Print();

    return 0;
}