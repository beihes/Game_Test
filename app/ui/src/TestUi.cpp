#include "../inc/TestUi.hpp"
#include "scene/inc/AppContext.hpp"
#include "scene/inc/AppScene.hpp"
#include "engine/component/tiled/inc/Layer.hpp"
#include <implot.h>
#include <spdlog/spdlog.h>

namespace app::ui {
    TestUi::TestUi(std::string_view name, scene::AppContext& appContext)
        :Ui(name, appContext.Get_Context()), appContext_(appContext) {
        spdlog::trace("[{}]ControlCameraUi 构建完成", this->Get_ClassName());
    }

    TestUi::~TestUi() {
        spdlog::trace("[{}]ControlCameraUi 析构完成", this->Get_ClassName());
    }

    void TestUi::Init() {
        this->runningState_ = true;
    }

    void TestUi::Input([[maybe_unused]] const SDL_Event& event) {

    }

    void TestUi::Render() {
        if (!this->runningState_) { return; }
        ImGui::ShowDemoWindow();
        if (ImGui::Begin(this->Get_Name().c_str(), &this->runningState_)) {
            if (ImPlot::BeginPlot("地图", ImVec2(-1, -1), ImPlotFlags_Equal | ImPlotFlags_NoTitle)) { // 设置标尺高度，宽度自适应
                ImPlot::SetupAxis(ImAxis_X1, nullptr, ImPlotAxisFlags_Opposite | ImPlotAxisFlags_NoDecorations);
                ImPlot::SetupAxis(ImAxis_Y1, nullptr, ImPlotAxisFlags_Invert | ImPlotAxisFlags_NoDecorations);
                // 设置坐标轴范围
                ImPlot::SetupAxisLimits(ImAxis_X1, 0, 640);
                ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 480);
                auto view = this->appContext_.appScene_.registry_.view<engine::component::tiled::Layer>();
                std::vector<std::reference_wrapper<engine::component::tiled::Layer>> layerVector;
                for (auto [entity, layer] : view.each()) {
                    auto it = std::lower_bound(layerVector.begin(), layerVector.end(), layer, [](const auto& lhs, const auto& rhs) {
                        return lhs.get().id_ < rhs.id_;
                        });
                    layerVector.insert(it, layer);
                }
                for (auto& v : layerVector) {
                    v.get().Render(this->appContext_.Get_Context());
                }
                // ImPlot::PlotImage("map1", (ImTextureID)it.second, ImPlotPoint(0, it.second->h), ImPlotPoint(it.second->w, 0));
                ImVec2 plotSize = ImPlot::GetPlotSize();
                ImPlot::EndPlot();
            }
        }
        ImGui::End();
    }

    void TestUi::Update() {

    }

    void TestUi::Clean() {

    }
} // namespace app::ui
