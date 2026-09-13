#pragma once

#ifndef tiled_tiled_hpp_
#define tiled_tiled_hpp_

#include "Map.hpp"
#include "ObjectGroup.hpp"
#include "Tile.hpp"
#include "TileLayer.hpp"
#include "TileSet.hpp"

namespace tiled {
    bool Load_JsonData(const std::string& source, nlohmann::json& jsonData);
} // namespace tiled


#endif