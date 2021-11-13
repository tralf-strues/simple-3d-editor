/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file editor_app.cpp
 * @date 2021-11-07
 * 
 * @copyright Copyright (c) 2021
 */

#include "sgl/core.h"
#include "sgl/scene/background.h"
#include "editor_app.h"

// const Sgl::ColorFill  MENU_BAR_FILL       = {Sml::COLOR_GREEN};
// const Sgl::Background MENU_BAR_BACKGROUND = {{&MENU_BAR_FILL}, {}};

int main(int argc, const char* argv[])
{
    EditorApplication application{argc, argv};

    return application.run();
}

void EditorApplication::onInit()
{
    LOG_INFO("Application initialization started.");

    Sgl::setContextRenderer(&m_Renderer);
    Sgl::DefaultSkins::g_DefaultFont = new Sml::Font("res/LucidaGrande.ttf", 16);

    class ButtonListener : public Sgl::ActionListener
    {
    public:
        Sgl::Button& button;

        ButtonListener(Sgl::Button& button) : button(button) {}

        virtual void onAction(Sgl::ActionEvent* event) override
        {
            LOG_INFO("Button '%s' pressed!", button.getLabel());
        }
    };

    m_MenuBar.setPrefWidth(EDITOR_WINDOW_WIDTH);
    Sgl::Menu* fileMenu = m_MenuBar.addMenu("File");
    Sgl::Menu* editMenu = m_MenuBar.addMenu("Edit");
    m_MenuBar.pushBackSpacer();
    Sgl::Menu* quitMenu = m_MenuBar.addMenu("Quit");

    fileMenu->getContextMenu()->addChild(new Sgl::MenuItem("File item 1"));
    fileMenu->getContextMenu()->addChild(new Sgl::MenuItem("File item 2"));
    fileMenu->getContextMenu()->addChild(new Sgl::MenuItem("File item 3"));
    fileMenu->getContextMenu()->addChild(new Sgl::MenuItem("File item 4"));

    // m_ButtonFile = new Sgl::Button("File");
    // m_ButtonFile->setOnAction(new ButtonListener(*m_ButtonFile));
    
    // m_ButtonEdit = new Sgl::Button("Edit");
    // m_ButtonEdit->setOnAction(new ButtonListener(*m_ButtonEdit));

    // m_ButtonQuit = new Sgl::Button("Quit");
    // m_ButtonQuit->setOnAction(new ButtonListener(*m_ButtonQuit));

    // m_BoxContainer = new Sgl::BoxContainer(Sgl::BoxContainer::Direction::LEFT_TO_RIGHT);
    // m_BoxContainer->setPrefWidth(EDITOR_WINDOW_WIDTH);
    // m_BoxContainer->setFillAcross(true);
    // m_BoxContainer->setBackground(&MENU_BAR_BACKGROUND);
    // m_BoxContainer->setLayoutY(15);
    // m_BoxContainer->addChild(m_ButtonFile);
    // m_BoxContainer->addChild(m_ButtonEdit);
    // m_BoxContainer->pushBackSpacer();
    // m_BoxContainer->addChild(m_ButtonQuit);

    m_SceneRoot.addChild(&m_MenuBar);
    m_Scene.setRoot(&m_SceneRoot);

    LOG_INFO("Application initialization finished.");
}

int EditorApplication::onQuit()
{
    LOG_INFO("Application quitting.");

    delete Sgl::DefaultSkins::g_DefaultFont;

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