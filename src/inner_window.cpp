/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file inner_window.cpp
 * @date 2021-11-15
 * 
 * @copyright Copyright (c) 2021
 */

#include "sml/sml_log.h"
#include "inner_window.h"

class MenuBarListener : public Sgl::ComponentEventListener<Sgl::MenuBar>
{
public:
    DEFINE_STATIC_LISTENED_EVENT_TYPES(Sml::MouseButtonPressedEvent::getStaticType())

public:
    MenuBarListener(Sgl::MenuBar* menuBar) : Sgl::ComponentEventListener<Sgl::MenuBar>(menuBar) {}

    virtual void onEvent(Sml::Event* event) override
    {
        LOG_APP_INFO("InnerWindow::MenuBarListener on MouseButtonPressedEvent");

        getComponent()->requestDrag();

        event->consume();
    }
};

class WindowDragListener : public Sgl::DragListener<Sgl::MenuBar>
{
public:
    DEFINE_STATIC_LISTENED_EVENT_TYPES_FROM_BASE_CLASS(Sgl::DragListener<Sgl::MenuBar>)

public:
    WindowDragListener(InnerWindow* window, Sgl::MenuBar* menuBar)
        : Sgl::DragListener<Sgl::MenuBar>(menuBar), m_Window(window) {}

    virtual void onDragMove(Sgl::DragMoveEvent* event) override
    {
        LOG_APP_INFO("WindowDragListener::onDragMove<%d, %d>", event->getDeltaX(), event->getDeltaY());

        m_Window->setLayoutX(m_Window->getLayoutX() + event->getDeltaX());
        m_Window->setLayoutY(m_Window->getLayoutY() + event->getDeltaY());
    }

private:
    InnerWindow* m_Window = nullptr;
};

class CloseButtonHoverListener : public Sgl::HoverListener<Sgl::Button>
{
public:
    DEFINE_STATIC_LISTENED_EVENT_TYPES_FROM_BASE_CLASS(Sgl::HoverListener<Sgl::Button>)

public:
    CloseButtonHoverListener(Sgl::Button* button) : Sgl::HoverListener<Sgl::Button>(button) {}

    virtual void onMouseEntered(Sgl::MouseEnteredEvent* event)
    {
        getComponent()->setIcon(Resources::ICON_CLOSE_HOVERED);
    }

    virtual void onMouseExited(Sgl::MouseExitedEvent* event)
    {
        getComponent()->setIcon(Resources::ICON_CLOSE_IDLE);
    }
};

const Sgl::Border InnerWindow::DEFAULT_BORDER = {1, 0xE9'E9'E9'FF};

InnerWindow::InnerWindow(const char* title, Sgl::Scene* scene)
    : m_Title(title)
{
    setScene(scene);
    setFillAcross(true);
    setBorder(&DEFAULT_BORDER);

    m_TitleLabel = new Sgl::Text(*Sgl::DefaultSkins::g_DefaultFont, title);
    m_CloseButton = new Sgl::Button(new Sgl::DefaultSkins::ButtonPlaneSkin, Resources::ICON_CLOSE_IDLE);
    m_CloseButton->getEventDispatcher()->attachHandler(CloseButtonHoverListener::EVENT_TYPES,
                                                       new CloseButtonHoverListener(m_CloseButton));

    m_MenuBar = new Sgl::MenuBar(scene);
    m_MenuBar->setFillAcross(false);
    m_MenuBar->pushBackSpacer();
    m_MenuBar->addChild(m_TitleLabel);
    m_MenuBar->pushBackSpacer();
    m_MenuBar->addChild(m_CloseButton);
    m_MenuBar->getEventDispatcher()->attachHandler(MenuBarListener::EVENT_TYPES,
                                                   new MenuBarListener(m_MenuBar));
    m_MenuBar->getEventDispatcher()->attachHandler(WindowDragListener::EVENT_TYPES,
                                                   new WindowDragListener(this, m_MenuBar));

    addChild(m_MenuBar);
}