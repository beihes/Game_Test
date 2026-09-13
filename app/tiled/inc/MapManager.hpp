#pragma once

#include <optional>
#include <typeinfo>

namespace app::scene {
    class AppContext;
} // namespace app::scene

namespace engine::component::tiled {
    class TileLayer;
} // namespace engine::component::tiled

namespace app::tiled {
    class MapManager {
    public:
        MapManager();
        ~MapManager();
    public:
        void Init(scene::AppContext& appContext);
        void Update(float deltaTime);
        void Render();
        void Clear();
    private:
        std::optional<std::reference_wrapper<scene::AppContext>> appContext_;
    private:
        const char* Get_ClassName() const {
            return typeid(*this).name();
        }
    };
}