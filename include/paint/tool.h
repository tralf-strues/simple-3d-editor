/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file tool.h
 * @date 2021-12-05
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "sml/sml_math.h"

namespace Paint
{
    class Tool
    {
    public:
        virtual ~Tool() {}

        virtual const char* getName() const = 0;
        virtual const char* getIconFilename() const = 0;

        virtual void onActionBegin(const Sml::Vec2i& pos) = 0;
        virtual void onAction(const Sml::Vec2i& pos, const Sml::Vec2i& displacement) = 0;
        virtual void onActionEnd(const Sml::Vec2i& pos) = 0;
    };
};