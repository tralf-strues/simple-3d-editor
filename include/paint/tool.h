/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file tool.h
 * @date 2021-12-05
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "sml/sml_math.h"
#include "sml/sml_graphics_wrapper.h"

namespace Paint
{
    class Tool
    {
    public:
        virtual ~Tool() = default;

        virtual const char* getName() const = 0;
        virtual const char* getIconFilename() const = 0;

        virtual void onActionStart(const Sml::Vec2i& pos) {}
        virtual void onAction(const Sml::Vec2i& pos, const Sml::Vec2i& displacement) {}
        virtual void onActionEnd(const Sml::Vec2i& pos) {}
    };
};