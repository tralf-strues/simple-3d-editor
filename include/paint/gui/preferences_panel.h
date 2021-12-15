/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file preferences_panel.h
 * @date 2021-12-15
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "sgl/scene/containers/container.h"
#include "../paint_editor.h"

namespace Paint
{
    class PreferencesPanel
    {
    public:
        static const Sgl::ColorFill  DEFAULT_FILL;
        static const Sgl::Background DEFAULT_BACKGROUND;
    
    public:
        PreferencesPanel();
        ~PreferencesPanel() = default;

        Sgl::Container* getView();
        void update();

    private:
        Sgl::Container* m_View = nullptr;
        Tool*           m_Tool = nullptr;
    };
}