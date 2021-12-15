/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file editor_app.cpp
 * @date 2021-11-07
 * 
 * @copyright Copyright (c) 2021
 */

#include <filesystem>
#include <dlfcn.h>
#include "sml/sml_log.h"
#include "sgl/scene/containers/tile_pane.h"
#include "sgl/scene/controls/scroll_bar.h"
#include "sgl/scene/controls/slider.h"
#include "sgl/scene/controls/scroll_pane.h"
#include "paint/plugin/api_impl.h"
#include "paint/plugin/plugin_tool.h"
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

    initPlugins();
}

void EditorApplication::initPlugins()
{
    LOG_APP_INFO("Plugins initialization started.");

    plugin::APIImpl api = {};

    for (auto entry : std::filesystem::directory_iterator("res/plugins"))
    {
        if (std::filesystem::path(entry.path()).extension().compare(".so") == 0)
        {
            LOG_APP_INFO("Plugin '%s' detected.", entry.path().c_str());

            void* handle = dlopen(entry.path().c_str(), RTLD_NOW);
            plugin::CreateFunction create = reinterpret_cast<plugin::CreateFunction>(dlsym(handle, "Create"));

            plugin::IPlugin* plugin = create(&api);
            plugin::Tools tools = plugin->GetTools();

            for (uint32_t i = 0; i < tools.count; ++i)
            {
                Paint::Editor::getInstance().addTool(new Paint::PluginTool(tools.tools[i]));
            }
        }
    }

    LOG_APP_INFO("Plugins initialization finished.");
}

void EditorApplication::initView()
{
    m_Scene      = new Sgl::Scene(m_Window.getWidth(), m_Window.getHeight());
    m_SceneRoot  = new Sgl::VBox();
    m_EditorPane = new Sgl::AnchorPane();
    m_SceneRoot->setFillAcross(true);
    m_Scene->setRoot(m_SceneRoot);

    initMenuBar();

    m_EditorPane->setAutoAdjust(true);
    m_SceneRoot->addChild(m_EditorPane);
    m_SceneRoot->setGrowPriority(m_EditorPane, Sgl::BoxContainer::GrowPriority::ALWAYS);

    initToolPanel();
    initPreferencesPanel();

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

    Sgl::ImageView* imageView = new Sgl::ImageView(new Sgl::Image("res/San_Francisco.png", Sgl::ImageFormat::PNG), true);
    Sgl::ScrollPane* scrollPane = new Sgl::ScrollPane();
    scrollPane->setLayoutX(100);
    scrollPane->setLayoutY(100);
    scrollPane->setPrefWidth(300);
    scrollPane->setPrefHeight(200);
    scrollPane->setContent(imageView);
    m_EditorPane->addChild(scrollPane);
}

void EditorApplication::initMenuBar()
{
    m_SceneRoot->addChild(m_MenuBar = new Sgl::MenuBar(m_Scene));

    m_MenuBar->setPrefWidth(EDITOR_WINDOW_WIDTH);
    Sgl::Menu* fileMenu  = m_MenuBar->addMenu("File");
    Sgl::Menu* editMenu  = m_MenuBar->addMenu("Edit");
    Sgl::Menu* toolsMenu = m_MenuBar->addMenu("Tools");

    /* File->New */
    class FileNewListener : public Sgl::ActionListener<Sgl::MenuItem>
    {
    public:
        FileNewListener(Sgl::MenuItem* menuItem, Sgl::AnchorPane* editorPane)
            : Sgl::ActionListener<Sgl::MenuItem>(menuItem), m_EditorPane(editorPane) {}

        virtual void onAction(Sgl::ActionEvent* event) override
        {
            Paint::Document* document = new Paint::Document(640, 360);
            Paint::Editor::getInstance().addDocument(document);
            Paint::Editor::getInstance().setActiveDocument(document);

            Paint::DocumentView* documentView = new Paint::DocumentView(document, getComponent()->getScene());

            InnerWindow* component = documentView->getView();
            m_EditorPane->addChild(documentView->getView());

            component->setLayoutX((component->getScene()->getWidth()  - component->computePrefWidth())  / 2);
            component->setLayoutY((component->getScene()->getHeight() - component->computePrefHeight()) / 2);
        }
    private:
        Sgl::AnchorPane* m_EditorPane;
    };

    Sgl::MenuItem* fileNewItem = new Sgl::MenuItem("New");
    fileNewItem->setOnAction(new FileNewListener(fileNewItem, m_EditorPane));
    fileMenu->getContextMenu()->addChild(fileNewItem);

    /* Tools */
    for (auto tool : Paint::Editor::getInstance().getTools())
    {
        toolsMenu->getContextMenu()->addChild(new Sgl::MenuItem(tool->getName()));
    }

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
    m_EditorPane->addChild(m_ToolPanel->getView());

    m_EditorPane->setTopAnchor(m_ToolPanel->getView(), 1);
    m_EditorPane->setBottomAnchor(m_ToolPanel->getView(), 1);
    m_EditorPane->setVerticalRelativePositioning(m_ToolPanel->getView(), true);

    m_EditorPane->setLeftAnchor(m_ToolPanel->getView(), 5);

    m_ToolPanel->loadTools();
}

void EditorApplication::initPreferencesPanel()
{
    m_PreferencesPanel = new Paint::PreferencesPanel();
    m_PreferencesPanel->update();
    
    m_EditorPane->addChild(m_PreferencesPanel->getView());

    m_EditorPane->setTopAnchor(m_PreferencesPanel->getView(), 0);
    m_EditorPane->setBottomAnchor(m_PreferencesPanel->getView(), 0);
    m_EditorPane->setRightAnchor(m_PreferencesPanel->getView(), 0);
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

    m_PreferencesPanel->update();

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