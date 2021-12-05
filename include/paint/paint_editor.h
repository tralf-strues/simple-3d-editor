/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file paint_editor.h
 * @date 2021-12-05
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include <list>
#include "tool.h"

namespace Paint
{
    class PaintEditor
    {
    public:

    private:
        Tool* m_SelectedTool = nullptr;
        std::list<Tool*> m_Tools;
    };
};