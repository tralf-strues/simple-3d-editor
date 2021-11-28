/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file resources.cpp
 * @date 2021-11-15
 * 
 * @copyright Copyright (c) 2021
 */

#include "resources.h"

Sgl::Image* Resources::ICON_CLOSE_IDLE    = nullptr;
Sgl::Image* Resources::ICON_CLOSE_HOVERED = nullptr;

void Resources::load()
{
    ICON_CLOSE_IDLE    = new Sgl::Image("res/icons/close_idle_32.png",    Sgl::ImageFormat::PNG);
    ICON_CLOSE_HOVERED = new Sgl::Image("res/icons/close_hovered_32.png", Sgl::ImageFormat::PNG);
}

void Resources::close()
{
    delete ICON_CLOSE_IDLE;
    delete ICON_CLOSE_HOVERED;
}