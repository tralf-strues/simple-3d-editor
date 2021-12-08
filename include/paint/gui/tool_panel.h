/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file tool_panel.h
 * @date 2021-12-06
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "sgl/scene/containers/box_container.h"
#include "sgl/scene/containers/tile_pane.h"
#include "paint/gui/color_picker_widget.h"

namespace Paint
{
    class ToolPanel
    {
    public:
        static const Sgl::Insets     DEFAULT_PADDING;
        static const int32_t         DEFAULT_SPACING;
        static const Sgl::Border     DEFAULT_BORDER;
        static const Sgl::ColorFill  DEFAULT_BACKGROUND_FILL;
        static const Sgl::Background DEFAULT_BACKGROUND;
        static const int32_t         DEFAULT_HGAP;
        static const int32_t         DEFAULT_VGAP;

    public:
        ToolPanel();
        ~ToolPanel();

        Sgl::VBox* getView();
        void loadTools();

    private:
        Sgl::VBox*         m_View        = nullptr;
        Sgl::TilePane*     m_Tools       = nullptr;
        ColorPickerWidget* m_ColorPicker = nullptr;
    };
};