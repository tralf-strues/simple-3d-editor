/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file color_picker_widget.h
 * @date 2021-12-07
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "sgl/scene/controls/control.h"
#include "../paint_editor.h"

namespace Paint
{
    class ColorPickerWidget : public Sgl::Control
    {
    public:
        ColorPickerWidget();
        virtual ~ColorPickerWidget() override = default;
    };
};