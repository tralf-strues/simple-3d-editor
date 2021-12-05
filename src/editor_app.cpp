/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file editor_app.cpp
 * @date 2021-11-07
 * 
 * @copyright Copyright (c) 2021
 */

#include "sml/sml_log.h"
#include "sgl/scene/background.h"
#include "sgl/scene/containers/tile_pane.h"
#include "sgl/scene/controls/slider.h"
#include "editor_app.h"

class DraggableButtonListener : public Sgl::DragListener<Sgl::Button>
{
public:
    DEFINE_STATIC_LISTENED_EVENT_TYPES_FROM_BASE_CLASS(Sgl::DragListener<Sgl::Button>)

public:
    DraggableButtonListener(Sgl::Button* button) : Sgl::DragListener<Sgl::Button>(button) {}

    virtual void onDragStart(Sgl::DragStartEvent* event) override
    {
        LOG_APP_INFO("DragStartEvent triggered");
    }

    virtual void onDragEnd(Sgl::DragEndEvent* event) override
    {
        LOG_APP_INFO("DragEndEvent triggered");
    }

    virtual void onDragMove(Sgl::DragMoveEvent* event) override
    {
        Sgl::Button* button = dynamic_cast<Sgl::Button*>(event->getTarget());
        button->setLayoutX(button->getLayoutX() + event->getDeltaX());
        button->setLayoutY(button->getLayoutY() + event->getDeltaY());

        LOG_APP_INFO("DragMoveEvent<%d, %d> triggered", event->getDeltaX(), event->getDeltaY());
    }
};

int main(int argc, const char* argv[])
{
    EditorApplication application{argc, argv};

    return application.run();
}

void EditorApplication::onInit()
{
    LOG_APP_INFO("Application initialization started.");

    Sml::Renderer::init(&m_Window);
    Sgl::DefaultSkins::g_DefaultFont = new Sml::Font("res/LucidaGrande.ttf", 16);
    ResourceManager::init("res/");

    m_SceneRoot = new Sgl::AnchorPane();

    m_MenuBar = new Sgl::MenuBar(&m_Scene);

    m_MenuBar->setPrefWidth(EDITOR_WINDOW_WIDTH);
    Sgl::Menu* fileMenu = m_MenuBar->addMenu("File");
    Sgl::Menu* editMenu = m_MenuBar->addMenu("Edit");
    m_MenuBar->pushBackSpacer();
    Sgl::Menu* quitMenu = m_MenuBar->addMenu("Quit");

    fileMenu->getContextMenu()->addChild(new Sgl::MenuItem("File item 1"));
    fileMenu->getContextMenu()->addChild(new Sgl::MenuItem("File item 2"));
    fileMenu->getContextMenu()->addChild(new Sgl::MenuItem("File item 3"));
    fileMenu->getContextMenu()->addChild(new Sgl::MenuItem("File item 4"));

    editMenu->getContextMenu()->addChild(new Sgl::MenuItem("Edit item 1"));
    editMenu->getContextMenu()->addChild(new Sgl::MenuItem("Edit item 2"));
    editMenu->getContextMenu()->addChild(new Sgl::MenuItem("Edit item 3"));
    editMenu->getContextMenu()->addChild(new Sgl::MenuItem("Edit item 4"));
    editMenu->getContextMenu()->addChild(new Sgl::MenuItem("Edit item 5"));
    editMenu->getContextMenu()->addChild(new Sgl::MenuItem("Edit item 6"));

    class DraggableButtonActionListener : public Sgl::ActionListener<Sgl::Button>
    {
        virtual void onAction(Sgl::ActionEvent* event) override
        {
            LOG_APP_INFO("ActionEvent triggered on m_DraggableButton");
            getComponent()->requestDrag();
        }
    };

    m_DraggableButton = new Sgl::Button("Drag me if you dare!");
    m_DraggableButton->setLayoutX(300);
    m_DraggableButton->setLayoutY(100);
    m_DraggableButton->setOnAction(new DraggableButtonActionListener());
    m_DraggableButton->setScene(&m_Scene); // FIXME:
    m_DraggableButton->getEventDispatcher()->attachHandler(DraggableButtonListener::EVENT_TYPES,
                                                           new DraggableButtonListener(m_DraggableButton));

    m_InnerWindow = new InnerWindow("Inner Window", &m_Scene);
    m_InnerWindow->setLayoutX(450);
    m_InnerWindow->setLayoutY(150);
    m_InnerWindow->setPrefWidth(400);
    // m_InnerWindow->setPrefHeight(500);

    // Sgl::ColorFill* colorFill = new Sgl::ColorFill(Sml::COLOR_RED);
    // Sgl::Background* background = new Sgl::Background(colorFill);

    Canvas* canvas = new Canvas{300, 300};
    canvas->setPrefHeight(400);
    // canvas->setBackground(background);
    m_InnerWindow->addChild(canvas);
    m_InnerWindow->setGrowPriority(canvas, Sgl::BoxContainer::GrowPriority::ALWAYS);

    class MouseMoveListener : public Sml::Listener
    {
    public:
        virtual void onEvent(Sml::Event* event) override
        {
            Sml::MouseMovedEvent* mouseMovedEvent = dynamic_cast<Sml::MouseMovedEvent*>(event);
            LOG_APP_INFO("MouseMovedEvent: <%d, %d>\n", mouseMovedEvent->getX(), mouseMovedEvent->getY());
        }
    };

    Sgl::TilePane* tilePane = new Sgl::TilePane(2);
    m_SceneRoot->addChild(tilePane);
    m_SceneRoot->setLeftAnchor(tilePane, 10);
    tilePane->setLayoutX(50);
    tilePane->setLayoutY(200);
    tilePane->addChild(new Sgl::Button("1"));
    tilePane->addChild(new Sgl::Button("2"));
    tilePane->addChild(new Sgl::Button("3"));
    tilePane->addChild(new Sgl::Button("4"));
    tilePane->addChild(new Sgl::Button("5"));
    tilePane->addChild(new Sgl::Button("6"));
    tilePane->addChild(new Sgl::Button("7"));
    tilePane->addChild(new Sgl::Button("8"));
    tilePane->addChild(new Sgl::Button("9"));

    Sgl::Slider* slider = new Sgl::Slider(-10, 10);
    slider->setPrefWidth(100);
    slider->setLayoutX(300);
    slider->setLayoutY(300);

    static Sgl::ShadowSpecification sliderShadow{{0, 0}, {1, 1}, 3, 0x11'11'11'66};
    slider->setShadow(&sliderShadow);

    class SliderListener : public Sml::PropertyChangeListener<float>
    {
    public:
        virtual void onPropertyChange(Sml::PropertyChangeEvent<float>* event) override
        {
            LOG_APP_INFO("SliderListener::onPropertyChange(%f -> %f)", event->getOldValue(), event->getNewValue());
        }
    };

    slider->setOnPropertyChange(new SliderListener());

    m_SceneRoot->addChild(m_DraggableButton);
    m_SceneRoot->addChild(m_MenuBar);
    m_SceneRoot->addChild(m_InnerWindow);
    m_SceneRoot->addChild(slider);

    // m_SceneRoot->getEventDispatcher()->attachHandler({Sml::MouseMovedEvent::getStaticType()},
    //                                                   new MouseMoveListener());

    m_Scene.setRoot(m_SceneRoot);

    LOG_APP_INFO("Application initialization finished.");
}

int EditorApplication::onQuit()
{
    LOG_APP_INFO("Application quitting.");

    delete Sgl::DefaultSkins::g_DefaultFont;

    LOG_APP_INFO("Application quit.");
    
    return 0;
}

void EditorApplication::onUpdate()
{
    proccessSystemEvents();

    m_Scene.update();
    m_Scene.render(m_Scene.getLayoutRegion());

    Sml::Renderer::getInstance().present();

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
            // LOG_APP_INFO("Skipping event (EventType = %" PRIu32 ", EventCategory = %" PRIu64 ")",
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
            LOG_APP_INFO("WindowCloseEvent");

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