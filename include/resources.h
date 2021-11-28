/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file resources.h
 * @date 2021-11-15
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "sgl/scene/media/image.h"

class Resources
{
public:
    static Sgl::Image* ICON_CLOSE_IDLE;
    static Sgl::Image* ICON_CLOSE_HOVERED;

    static void load();
    static void close();
};