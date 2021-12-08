/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file background_foreground_switcher.h
 * @date 2021-12-07
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "sgl/scene/controls/control.h"
#include "../paint_editor.h"

namespace Paint
{
    class BackgroundForegroundSwitcher : public Sgl::Control
    {
    public:
        BackgroundForegroundSwitcher();
        virtual ~BackgroundForegroundSwitcher() override = default;
    };
};