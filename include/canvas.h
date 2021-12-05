/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file canvas.h
 * @date 2021-11-15
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "sgl/scene/containers/container.h"

class Canvas : public Sgl::Container
{
public:
    static const Sgl::ColorFill  BACKGROUND_FILL;
    static const Sgl::Background BACKGROUND;

public:
    Canvas(int32_t width, int32_t height);
    virtual ~Canvas() override;

    Sml::Texture* getTexture();
    Sml::Vec2i getTexturePos() const;

private:
    Sml::Texture* m_Texture = nullptr;

    virtual void prerenderSelf() override;

    virtual int32_t computeCustomPrefWidth(int32_t height = -1) const override;
    virtual int32_t computeCustomPrefHeight(int32_t width = -1) const override;
};