#pragma once

#ifndef tiled_tiled_hpp_
#define tiled_tiled_hpp_

#include "src/inc/Map.hpp"
#include "src/inc/ObjectGroup.hpp"
#include "src/inc/Tile.hpp"
#include "src/inc/TileLayer.hpp"
#include "src/inc/TileSet.hpp"

namespace tiled {
    bool Load_JsonData(const std::string& source, nlohmann::json& jsonData);
} // namespace tiled


#endif