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

namespace Paint
{
    class ToolPanel
    {
    public:
        ToolPanel();
        ~ToolPanel();

        Sgl::VBox* getView();
        void loadTools();

    private:
        Sgl::VBox*     m_View  = nullptr;
        Sgl::TilePane* m_Tools = nullptr;
    };
};