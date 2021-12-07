/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file basic_tools.h
 * @date 2021-12-06
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "tool.h"

namespace Paint
{
    class Brush : public Tool
    {
    public:
        virtual const char* getName() const override;
        virtual const char* getIconFilename() const override;

        virtual void onAction(const Sml::Vec2i& pos, const Sml::Vec2i& displacement) override;
    };
};