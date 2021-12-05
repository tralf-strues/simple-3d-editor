/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file editor_app.h
 * @date 2021-11-07
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "sml/graphics_wrapper/renderer.h"
#include "sml/events/system_event_manager.h"
#include "sml/app/app.h"
#include "sgl/scene/scene.h"
#include "sgl/scene/containers/anchor_pane.h"
#include "sgl/scene/controls/button.h"
#include "sgl/scene/containers/menu_bar.h"
#include "resources.h"
#include "inner_window.h"
#include "canvas.h"

constexpr size_t      EDITOR_WINDOW_WIDTH            = 1280;
constexpr size_t      EDITOR_WINDOW_HEIGHT           = 720;
constexpr const char* EDITOR_WINDOW_TITLE            = "Simple 3d Editor";
constexpr size_t      EDITOR_MAX_WINDOW_TITLE_LENGTH = 64;

class EditorApplication : public Sml::Application
{
public:
    EditorApplication(int32_t argc, const char* argv[])
        : Application(argc, argv),
          m_Window(EDITOR_WINDOW_WIDTH, EDITOR_WINDOW_HEIGHT, EDITOR_WINDOW_TITLE),
          m_Scene(m_Window.getWidth(), m_Window.getHeight())
    {}

    virtual void onInit() override;
    virtual int onQuit() override;
    virtual void onUpdate() override;

private:
    Sml::Window             m_Window;
    Sml::SystemEventManager m_SystemEventManager;

    char                    m_WindowTitle[EDITOR_MAX_WINDOW_TITLE_LENGTH];

    Sgl::Scene              m_Scene;
    Sgl::AnchorPane*        m_SceneRoot;

    Sgl::MenuBar*           m_MenuBar;
    Sgl::Button*            m_DraggableButton = nullptr;
    InnerWindow*            m_InnerWindow     = nullptr;

    void proccessSystemEvents();
    void proccessWindowEvent(Sml::WindowEvent* event);
    void proccessInputEvent(Sml::Event* event);

    void updateWindowTitle();
};