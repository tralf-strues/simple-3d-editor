/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file plugin_impl.h
 * @date 2021-11-24
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "sml/sml_graphics_wrapper.h"
#include "plugins_api/plugin.hpp"

namespace Sgl
{
    class TextureImpl : public ITexture
    {
    public:
        virtual ~TextureImpl() override = default;

        virtual int32_t GetWidth() override;
        virtual int32_t GetHieght() override;

        virtual Buffer ReadBuffer() override;
        virtual void ReleaseBuffer(const Buffer& buffer) override;
        virtual void LoadBuffer(const Buffer& buffer) override;

        virtual void Clear(Color color) override;
        virtual void Present() override;

        virtual void DrawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1, Color color) override;
        virtual void DrawThickLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t thickness, Color color) override;
        virtual void DrawCircle(int32_t x, int32_t y, int32_t radius, Color color) override;
        virtual void DrawRect(int32_t x, int32_t y, int32_t width, int32_t height, Color color) override;

        virtual void CopyTexture(ITexture* texture, int32_t x, int32_t y, int32_t width, int32_t height) override;

    private:
        
    };
}