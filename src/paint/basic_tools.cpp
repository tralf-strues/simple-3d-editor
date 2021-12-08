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