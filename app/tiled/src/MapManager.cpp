#include "../inc/MapManager.hpp"
#include "../inc/Map.hpp"
#include <scene/inc/AppContext.hpp>
#include <scene/inc/AppScene.hpp>
#include <spdlog/spdlog.h>

namespace app::tiled {
    MapManager::MapManager() {
        spdlog::trace("[{}]MapManager 构建完成", this->Get_ClassName());
    }

    MapManager::~MapManager() {
        this->Clear();
        spdlog::trace("[{}]MapManager 析构完成", this->Get_ClassName());
    }

    void MapManager::Init(scene::AppContext& appContext) {
        this->appContext_ = appContext;
        auto& midMap = appContext.appScene_.registry_.ctx().emplace<Map>();
        midMap.Init("tiled/maps", "Beach.tmj", appContext);

    }

    void MapManager::Update(float deltaTime) {

    }

    void MapManager::Render() {

    }

    void MapManager::Clear() {

    }
} // namespace app::tiled
