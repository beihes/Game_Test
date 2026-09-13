#include "../inc/tilelayer.hpp"
#include "scene/inc/AppContext.hpp"
#include "scene/inc/AppScene.hpp"
#include "engine/component/tiled/inc/Layer.hpp"
#include "engine/core/inc/Context.hpp"
#include "engine/render/inc/Sprite.hpp"
#include "engine/resource/inc/ResourceManager.hpp"
#include "engine/resource/inc/TextureManager.hpp"
#include <imgui.h>
#include <implot.h>
#include <spdlog/spdlog.h>

namespace app::tiled::tilelayer {
    void Render_Callback(engine::component::tiled::Layer& tileLayer, void* userData) {
        auto* appContext = static_cast<scene::AppContext*>(userData);
        if (!appContext->Get_Context().Get_ResourceManager().Get_TextureManager()) {
            return;
        }
        ImDrawList* drawList = ImPlot::GetPlotDrawList();
        for (auto& sprite : tileLayer.spriteVector_) {
            if (sprite.textureName_.empty() || !sprite.srcRect_.has_value() || !sprite.dstRect_.has_value()) {
                continue;
            }
            SDL_Texture* texture = appContext->Get_Context().Get_ResourceManager().Get_TextureManager()->Search(sprite.textureName_);
            if (!texture) {
                continue;
            }
            if (tileLayer.texture_) {

            }


            float textureWidth = static_cast<float>(texture->w);
            float textureHeight = static_cast<float>(texture->h);
            const glm::vec4& source = sprite.srcRect_.value();
            const glm::vec4& destination = sprite.dstRect_.value();
            const ImVec2 plotMin = ImPlot::PlotToPixels(ImPlotPoint(destination.x, destination.y));
            const ImVec2 plotMax = ImPlot::PlotToPixels(ImPlotPoint(destination.x + destination.z, destination.y + destination.w));
            const ImVec2 uvMin(source.x / textureWidth, source.y / textureHeight);
            const ImVec2 uvMax((source.x + source.z) / textureWidth, (source.y + source.w) / textureHeight);

            drawList->AddImage((ImTextureID)texture, plotMin, plotMax, uvMin, uvMax);
        }

        /* // 1. 获取当前 Plot 的绘制列表
        ImDrawList* drawList = ImPlot::GetPlotDrawList();

        // 2. 把你想要的目标坐标系范围转换成屏幕像素坐标
        ImVec2 p_min = ImPlot::PlotToPixels(ImPlotPoint(x0, y1)); // 注意 Y 轴方向
        ImVec2 p_max = ImPlot::PlotToPixels(ImPlotPoint(x1, y0));

        // 3. 计算归一化 UV 坐标（源矩形）
        float texW = it.second->w; // 纹理实际宽度
        float texH = it.second->h; // 纹理实际高度
        ImVec2 uv_min(srcX / texW, srcY / texH);
        ImVec2 uv_max((srcX + srcW) / texW, (srcY + srcH) / texH);

        // 4. 手动绘制
        drawList->AddImage((ImTextureID)it.second, p_min, p_max, uv_min, uv_max); */

    }
} // namespace app::tiled::tilelayer
