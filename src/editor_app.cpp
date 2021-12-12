/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file editor_app.cpp
 * @date 2021-11-07
 * 
 * @copyright Copyright (c) 2021
 */

#include "sml/sml_log.h"
#include "sgl/scene/containers/tile_pane.h"
#include "sgl/scene/controls/slider.h"
#include "editor_app.h"

int main(int argc, const char* argv[])
{
    EditorApplication application{argc, argv};

    return application.run();
}

void EditorApplication::onInit()
{
    LOG_APP_INFO("Application initialization started.");

    initSystem();
    initEditor();
    initView();

    LOG_APP_INFO("Application initialization finished.");
}

void EditorApplication::initSystem()
{
    Sml::Renderer::init(&m_Window);
    Sgl::DefaultSkins::g_DefaultFont = new Sml::Font("res/LucidaGrande.ttf", 16);
    ResourceManager::init("res/");
}

void EditorApplication::initEditor()
{
    Paint::Editor::init();

    Paint::Editor& paintEditor = Paint::Editor::getInstance();
    
    Paint::Brush* brush = new Paint::Brush();
    paintEditor.addTool(brush);
    paintEditor.setActiveTool(brush);

    paintEditor.addTool(new Paint::Eraser());
}

const Sgl::LinearGradientFill MAIN_GRADIENT = Sgl::LinearGradientFill(Sgl::LinearGradientFill::Direction::HORIZONTAL,
                                                                      {{0.00, 0xFF'00'00'FF},
                                                                       {0.15, 0xFF'00'FF'FF},
                                                                       {0.33, 0x00'00'FF'FF},
                                                                       {0.49, 0x00'FF'FF'FF},
                                                                       {0.67, 0x00'FF'00'FF},
                                                                       {0.84, 0xFF'FF'00'FF},
                                                                       {1.00, 0xFF'00'00'FF}});

const Sgl::LinearGradientFill GRAY_GRADIENT = Sgl::LinearGradientFill(Sgl::LinearGradientFill::Direction::VERTICAL,
                                                                      {{0.00, 0xFF'FF'FF'FF},
                                                                       {0.50, 0x00'00'00'00},
                                                                       {1.00, 0x00'00'00'FF}});

void EditorApplication::initView()
{
    m_Scene     = new Sgl::Scene(m_Window.getWidth(), m_Window.getHeight());
    m_SceneRoot = new Sgl::AnchorPane();
    m_SceneRoot->setAutoAdjust(true);
    m_Scene->setRoot(m_SceneRoot);

    initMenuBar();
    initToolPanel();

    /* Mouse logger */
    // class MouseMoveListener : public Sml::Listener
    // {
    // public:
    //     virtual void onEvent(Sml::Event* event) override
    //     {
    //         Sml::MouseMovedEvent* mouseMovedEvent = dynamic_cast<Sml::MouseMovedEvent*>(event);
    //         LOG_APP_INFO("MouseMovedEvent: <%d, %d>", mouseMovedEvent->getX(), mouseMovedEvent->getY());
    //     }
    // };

    // m_SceneRoot->getEventDispatcher()->attachHandler({Sml::MouseMovedEvent::getStaticType()}, new MouseMoveListener());
}

void EditorApplication::initMenuBar()
{
    m_SceneRoot->addChild(m_MenuBar = new Sgl::MenuBar(m_Scene));

    m_MenuBar->setPrefWidth(EDITOR_WINDOW_WIDTH);
    Sgl::Menu* fileMenu = m_MenuBar->addMenu("File");
    Sgl::Menu* editMenu = m_MenuBar->addMenu("Edit");
    m_MenuBar->pushBackSpacer();

    /* File->New */
    class FileNewListener : public Sgl::ActionListener<Sgl::MenuItem>
    {
    public:
        FileNewListener(Sgl::MenuItem* menuItem) : Sgl::ActionListener<Sgl::MenuItem>(menuItem) {}

        virtual void onAction(Sgl::ActionEvent* event) override
        {
            Paint::Document* document = new Paint::Document(640, 360);
            Paint::Editor::getInstance().addDocument(document);
            Paint::Editor::getInstance().setActiveDocument(document);

            Paint::DocumentView* documentView = new Paint::DocumentView(document, getComponent()->getScene());

            InnerWindow* component = documentView->getView();
            getComponent()->getScene()->getRoot()->addChild(documentView->getView());

            component->setLayoutX((component->getScene()->getWidth()  - component->computePrefWidth())  / 2);
            component->setLayoutY((component->getScene()->getHeight() - component->computePrefHeight()) / 2);
        }
    };

    Sgl::MenuItem* fileNewItem = new Sgl::MenuItem("New");
    fileNewItem->setOnAction(new FileNewListener(fileNewItem));
    fileMenu->getContextMenu()->addChild(fileNewItem);

    fileMenu->getContextMenu()->addChild(new Sgl::MenuItem("File item 2"));
    fileMenu->getContextMenu()->addChild(new Sgl::MenuItem("File item 3"));
    fileMenu->getContextMenu()->addChild(new Sgl::MenuItem("File item 4"));

    editMenu->getContextMenu()->addChild(new Sgl::MenuItem("Edit item 1"));
    editMenu->getContextMenu()->addChild(new Sgl::MenuItem("Edit item 2"));
    editMenu->getContextMenu()->addChild(new Sgl::MenuItem("Edit item 3"));
    editMenu->getContextMenu()->addChild(new Sgl::MenuItem("Edit item 4"));
    editMenu->getContextMenu()->addChild(new Sgl::MenuItem("Edit item 5"));
    editMenu->getContextMenu()->addChild(new Sgl::MenuItem("Edit item 6"));
}

void EditorApplication::initToolPanel()
{
    m_ToolPanel = new Paint::ToolPanel();
    m_SceneRoot->addChild(m_ToolPanel->getView());

    m_SceneRoot->setTopAnchor(m_ToolPanel->getView(), 1);
    m_SceneRoot->setBottomAnchor(m_ToolPanel->getView(), 1);
    m_SceneRoot->setVerticalRelativePositioning(m_ToolPanel->getView(), true);

    m_SceneRoot->setLeftAnchor(m_ToolPanel->getView(), 5);

    m_ToolPanel->loadTools();
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

    m_Scene->update();
    m_Scene->render(m_Scene->getLayoutRegion());

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

    m_Scene->proccessEvent(event);
}

void EditorApplication::updateWindowTitle()
{
    snprintf(m_WindowTitle, EDITOR_MAX_WINDOW_TITLE_LENGTH,     
             "%s [FPS: %" PRIu32 "]", EDITOR_WINDOW_TITLE, getFps());

    m_Window.updateTitle(m_WindowTitle);
}