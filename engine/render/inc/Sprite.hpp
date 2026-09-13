#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#ifdef __cplusplus
}
#endif // __cplusplus

#include <glm/glm.hpp>
#include <string>
#include <optional>

namespace engine::render {
    class Sprite final {
    public:
        Sprite(const std::string& textureName = "", const std::optional<glm::vec4>& srcRect = std::nullopt, const std::optional<glm::vec4>& dstRect = std::nullopt, bool flipState = false);
        ~Sprite();
    public:
        void Set_Name(const std::string& textureName);
        void Set_SrcRect(const std::optional<glm::vec4>& srcRect);
        void Set_DstRect(const std::optional<glm::vec4>& dstRect);
        void Set_FlipState(bool flipState);
    public:
        const std::string& Get_Name()const;
        const std::optional<glm::vec4> Get_SrcRect()const;
        const bool Get_FlipState() const;
    public:
        std::string textureName_;/* 图像名字 */
        std::optional<glm::vec4> srcRect_;//纹理中要绘制的部分
        std::optional<glm::vec4> dstRect_;//绘制到目标纹理的区域的部分
        bool flipState_ = false;//是否水平翻转
    private:
        const char* Get_ClassName() const {
            return typeid(*this).name();
        }
    };
}
