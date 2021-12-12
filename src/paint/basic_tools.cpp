/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file basic_tools.cpp
 * @date 2021-12-06
 * 
 * @copyright Copyright (c) 2021
 */

#include "paint/basic_tools.h"
#include "paint/paint_editor.h"

using namespace Paint;

//------------------------------------------------------------------------------
// Brush
//------------------------------------------------------------------------------
const char* Brush::getName() const         { return "Brush"; }
const char* Brush::getIconFilename() const { return "icons/paintbrush.png"; }

void Brush::onAction(const Sml::Vec2i& pos, const Sml::Vec2i& displacement)
{
    Sml::Renderer& renderer = Sml::Renderer::getInstance();

    renderer.setColor(Editor::getInstance().getForeground());
    Sml::renderLine(pos - displacement, pos, m_Thickness);
}

int32_t Brush::getThickness() const         { return m_Thickness; }
void Brush::setThickness(int32_t thickness) { assert(thickness > 0); m_Thickness = thickness; }

//------------------------------------------------------------------------------
// Eraser
//------------------------------------------------------------------------------
const char* Eraser::getName() const { return "Eraser"; }
const char* Eraser::getIconFilename() const { return "icons/eraser.png"; }

void Eraser::onAction(const Sml::Vec2i& pos, const Sml::Vec2i& displacement)
{
    Sml::Renderer& renderer = Sml::Renderer::getInstance();

    renderer.setColor(Sml::COLOR_TRANSPARENT);
    renderer.setBlendMode(Sml::Renderer::BlendMode::NONE);

    Sml::renderLine(pos - displacement, pos, m_Thickness);

    renderer.setBlendMode(Sml::Renderer::BlendMode::BLEND);
}

int32_t Eraser::getThickness() const { return m_Thickness; }
void Eraser::setThickness(int32_t thickness) { m_Thickness = thickness; }