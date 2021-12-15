/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file texture_impl.cpp
 * @date 2021-12-14
 * 
 * @copyright Copyright (c) 2021
 */

#include "paint/plugin/texture_impl.h"

using namespace plugin;

TextureImpl::TextureImpl(Sml::Texture* texture) { assert(texture); m_Texture = texture; }

TextureImpl::~TextureImpl()
{
    // delete m_Texture; FIXME: WTF?
}

int32_t TextureImpl::GetSizeX() { return m_Texture->getWidth(); }
int32_t TextureImpl::GetSizeY() { return m_Texture->getHeight(); }

Buffer TextureImpl::ReadBuffer()
{
    assert(m_Texture);
    return {m_Texture->readPixels(nullptr), this};
}

void TextureImpl::ReleaseBuffer(Buffer buffer)
{
    delete buffer.pixels;
}

void TextureImpl::LoadBuffer(Buffer buffer)
{
    assert(this == buffer.texture);
    m_Texture->updatePixels(buffer.pixels);
}

void TextureImpl::Clear(color_t color)
{
    Sml::Renderer& renderer = Sml::Renderer::getInstance();
    renderer.pushSetTarget(m_Texture);

    renderer.setColor(color);
    renderer.clear();

    renderer.popTarget();
}

void TextureImpl::Present() {}

void TextureImpl::DrawLine(const Line& line)
{
    Sml::Renderer& renderer = Sml::Renderer::getInstance();
    renderer.pushSetTarget(m_Texture);

    renderer.setColor(line.color);
    Sml::renderLine({line.x0, line.y0}, {line.x1, line.y1}, line.thickness);

    renderer.popTarget();
}

void TextureImpl::DrawCircle(const Circle& circle)
{
    Sml::Renderer& renderer = Sml::Renderer::getInstance();
    renderer.pushSetTarget(m_Texture);

    renderer.setColor(circle.fill_color);
    Sml::renderFilledCircle({{circle.x, circle.y}, circle.radius});

    renderer.setColor(circle.outline_color);
    for (int32_t i = 0; i < circle.outline_thickness; ++i)
    {
        Sml::renderCircle({{circle.x, circle.y}, circle.radius - i});
    }

    renderer.popTarget();
}

void TextureImpl::DrawRect(const Rect& rect)
{
    Sml::Renderer& renderer = Sml::Renderer::getInstance();
    renderer.pushSetTarget(m_Texture);

    renderer.setColor(rect.fill_color);
    Sml::renderFilledRect({rect.x, rect.y, rect.size_x, rect.size_y});

    renderer.setColor(rect.outline_color);
    Sml::renderRect({rect.x, rect.y, rect.size_x, rect.size_y}, static_cast<uint8_t>(rect.outline_thickness));

    renderer.popTarget();
}

void TextureImpl::CopyTexture(ITexture* source, int32_t x, int32_t y, int32_t size_x, int32_t size_y)
{
    TextureImpl* sourceTexture = dynamic_cast<TextureImpl*>(source);

    Sml::Rectangle<int32_t> dstRect = {x, y, size_x, size_y};
    sourceTexture->m_Texture->copyTo(m_Texture, &dstRect, nullptr);
}

void TextureImpl::CopyTexture(ITexture* source, int32_t x, int32_t y)
{
    TextureImpl* sourceTexture = dynamic_cast<TextureImpl*>(source);

    Sml::Rectangle<int32_t> dstRect = {x, y, sourceTexture->GetSizeX(), sourceTexture->GetSizeY()};
    sourceTexture->m_Texture->copyTo(m_Texture, &dstRect, nullptr);
}

const Sml::Texture* TextureImpl::GetTexture() const { return m_Texture; }

ITexture* TextureFactoryImpl::CreateTexture(const char* filename)
{
    static char fullname[1024];
    snprintf(fullname, sizeof(fullname), "res/plugins/%s", filename);

    Sml::Texture* texture = new Sml::Texture();
    
    if (!texture->loadFromImage(fullname))
    {
        delete texture;
        return nullptr;
    }

    return new TextureImpl(texture);
}

ITexture* TextureFactoryImpl::CreateTexture(int32_t size_x, int32_t size_y)
{
    return new TextureImpl(new Sml::Texture(static_cast<size_t>(size_x), static_cast<size_t>(size_y)));
}