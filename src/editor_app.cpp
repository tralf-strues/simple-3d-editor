/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file editor_app.cpp
 * @date 2021-11-07
 * 
 * @copyright Copyright (c) 2021
 */

#include "editor_app.h"
#include "sgl/core.h"

int main(int argc, const char* argv[])
{
    EditorApplication application{argc, argv};

    return application.run();
}

void EditorApplication::onInit()
{
    LOG_INFO("Application initialization started.");

    Sgl::setContextRenderer(&m_Renderer);
    m_Scene.setRoot(&m_SceneRoot);

    LOG_INFO("Application initialization finished.");
}

int EditorApplication::onQuit()
{
    LOG_INFO("Application quit.");
    return 0;
}

void EditorApplication::onUpdate()
{
    proccessSystemEvents();

    m_Scene.update();
    m_Scene.render(m_Scene.getLayoutRegion());
    m_Renderer.present();

    updateWindowTitle();
}

void EditorApplication::proccessSystemEvents()
{
    Sml::Event* nextEvent = nullptr;

    while ((nextEvent = m_SystemEventManager.pollEvent(false)) != nullptr)
    {
        if (nextEvent->isInCategory(Sml::EVENT_CATEGORY_WINDOW))
        {
            proccessWindowEvent(static_cast<Sml::WindowEvent*>(nextEvent));
        }
        else if (nextEvent->isInCategory(Sml::EVENT_CATEGORY_INPUT))
        {
            proccessInputEvent(nextEvent);
        }
        else
        {
            // LOG_INFO("Skipping event (EventType = %" PRIu32 ", EventCategory = %" PRIu64 ")",
            //          nextEvent->getType(),
            //          nextEvent->getCategory());
        }

        delete nextEvent;
    }
}

void EditorApplication::proccessWindowEvent(Sml::WindowEvent* event)
{
    assert(event);

    switch (event->getType())
    {
        case Sml::WindowCloseEvent::getStaticType():
        {
            LOG_INFO("WindowCloseEvent");

            stop();
            break;
        }

        default: { break; }
    }
}

void EditorApplication::proccessInputEvent(Sml::Event* event)
{
    assert(event);

    m_Scene.proccessEvent(event);
}

void EditorApplication::updateWindowTitle()
{
    snprintf(m_WindowTitle, EDITOR_MAX_WINDOW_TITLE_LENGTH,     
             "%s [FPS: %" PRIu32 "]", EDITOR_WINDOW_TITLE, getFps());

    m_Window.updateTitle(m_WindowTitle);
}