/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file basic_tools.cpp
 * @date 2021-12-06
 * 
 * @copyright Copyright (c) 2021
 */

#include "paint/basic_tools.h"

using namespace Paint;

const char* Brush::getName() const         { return "Brush"; }
const char* Brush::getIconFilename() const { return "icons/paintbrush.png"; }

void Brush::onAction(const Sml::Vec2i& pos, const Sml::Vec2i& displacement)
{
    Sml::Renderer& renderer = Sml::Renderer::getInstance();

    renderer.setColor(Sml::COLOR_BLACK);
    Sml::renderLine(pos - displacement, pos);
}