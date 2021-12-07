/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file inner_window.h
 * @date 2021-11-15
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "sgl/scene/containers/box_container.h"
#include "sgl/scene/containers/menu_bar.h"
#include "resource_manager.h"

class InnerWindow : public Sgl::VBox
{
public:
    static const Sgl::Border DEFAULT_BORDER;
    static const char*       ICON_CLOSE_IDLE;
    static const char*       ICON_CLOSE_HOVERED;

public:
    InnerWindow(const char* title, Sgl::Scene* scene);
    virtual ~InnerWindow() = default;

    void updateTitle(const char* title);

private:
    const char*   m_Title       = nullptr;
    
    Sgl::MenuBar* m_MenuBar     = nullptr;
    Sgl::Text*    m_TitleLabel  = nullptr; 
    Sgl::Button*  m_CloseButton = nullptr;
};