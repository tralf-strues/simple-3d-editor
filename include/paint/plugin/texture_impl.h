/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file texture_impl.h
 * @date 2021-12-14
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "sml/sml_graphics_wrapper.h"
#include "plugin_api.h"

namespace plugin
{
    class TextureImpl : public ITexture
    {
    public:
        TextureImpl(Sml::Texture* texture);
        virtual ~TextureImpl() override;

        virtual int32_t GetSizeX() override;
        virtual int32_t GetSizeY() override;

        virtual Buffer ReadBuffer() override;
        virtual void ReleaseBuffer(Buffer buffer) override;
        virtual void LoadBuffer(Buffer buffer) override;

        virtual void Clear(color_t color) override;
        virtual void Present() override;

        virtual void DrawLine  (const Line& line) override;
        virtual void DrawCircle(const Circle& circle) override;
        virtual void DrawRect  (const Rect& rect) override;

        virtual void CopyTexture(ITexture* source, int32_t x, int32_t y, int32_t size_x, int32_t size_y) override;
        virtual void CopyTexture(ITexture* source, int32_t x, int32_t y) override;

        const Sml::Texture* GetTexture() const;

    private:
        Sml::Texture* m_Texture = nullptr;
    };

    class TextureFactoryImpl : public ITextureFactory
    {
    public:
        virtual ~TextureFactoryImpl() {}

        virtual ITexture* CreateTexture(const char* filename) override;
        virtual ITexture* CreateTexture(int32_t size_x, int32_t size_y) override;
    };
};