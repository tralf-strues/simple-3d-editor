/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file background_foreground_switcher.cpp
 * @date 2021-12-07
 * 
 * @copyright Copyright (c) 2021
 */

#include "sgl/scene/shapes/rectangle.h"
#include "sgl/scene/controls/button.h"
#include "sgl/scene/containers/context_menu.h"
#include "paint/gui/background_foreground_switcher.h"
#include "paint/gui/color_picker.h"
#include "paint/paint_editor.h"
#include "resource_manager.h"

using namespace Paint;

static const char*   ICON_SWITCH_BUTTON     = "icons/curved_arrow.png";
static const int32_t COLOR_PICKER_PREF_SIZE = 38;

class BackgroundForegroundSwitcherSkin : public Sgl::BaseSkin<BackgroundForegroundSwitcher>
{
public:
    enum class Type
    {
        BACKGROUND,
        FOREGROUND
    };

    class PickColorListener;

public:
    BackgroundForegroundSwitcherSkin(BackgroundForegroundSwitcher* widget) : m_Widget(widget) { attach(m_Widget); }
    
    void showColorPicker()
    {
        if (m_ColorPickerMenu == nullptr)
        {
            m_ColorPickerMenu = new Sgl::ContextMenu(m_Widget->getScene());

            m_ColorPicker = new ColorPicker();
            m_ColorPickerMenu->addChild(m_ColorPicker);
            m_ColorPickerMenu->setShadow(nullptr);

            class ColorChangeListener : public Sml::PropertyChangeListener<Sml::Color>
            {
            public:
                ColorChangeListener(Type& beingPicked) : m_BeingPicked(beingPicked) {}

                virtual void onPropertyChange(Sml::PropertyChangeEvent<Sml::Color>* event) override
                {
                    LOG_APP_INFO("Changing color from 0x%08" PRIx32 " to 0x%08" PRIx32,
                                 event->getOldValue(), event->getNewValue());

                    switch (m_BeingPicked)
                    {
                        case Type::BACKGROUND: { Editor::getInstance().setBackground(event->getNewValue()); break; }
                        case Type::FOREGROUND: { Editor::getInstance().setForeground(event->getNewValue()); break; }
                    }
                }

            private:
                Type& m_BeingPicked;
            };

            m_ColorPicker->setOnPropertyChange(new ColorChangeListener(m_BeingPicked));
        }

        switch (m_BeingPicked)
        {
            case Type::BACKGROUND: { m_ColorPicker->setColor(Editor::getInstance().getBackground()); break; }
            case Type::FOREGROUND: { m_ColorPicker->setColor(Editor::getInstance().getForeground()); break; }
        }

        m_ColorPickerMenu->show();
    }

    virtual void attach(BackgroundForegroundSwitcher* widget) override
    {
        assert(widget);

        m_Widget = widget;

        m_Widget->addChild(m_BackgroundRect = new Sgl::Rectangle());
        m_Widget->addChild(m_ForegroundRect = new Sgl::Rectangle());

        class PickColorListener : public Sml::Listener
        {
        public:
            PickColorListener(BackgroundForegroundSwitcherSkin* skin, Type type) : m_Skin(skin), m_Type(type) {}

            virtual void onEvent(Sml::Event* event) override
            {
                m_Skin->m_BeingPicked = m_Type;
                m_Skin->showColorPicker();
            }

        private:
            BackgroundForegroundSwitcherSkin* m_Skin = nullptr;
            const Type                        m_Type;
        };

        m_BackgroundRect->getEventDispatcher()->attachHandler({Sml::MouseButtonPressedEvent::getStaticType()},
                                                              new PickColorListener(this, Type::BACKGROUND));

        m_ForegroundRect->getEventDispatcher()->attachHandler({Sml::MouseButtonPressedEvent::getStaticType()},
                                                              new PickColorListener(this, Type::FOREGROUND));

        m_SwitchButton = new Sgl::Button(new Sgl::DefaultSkins::ButtonPlaneSkin(), RESOURCE_IMAGE(ICON_SWITCH_BUTTON));
        m_Widget->addChild(m_SwitchButton);

        class SwitchButtonListener : public Sgl::ActionListener<Sgl::Button>
        {
        public:
            SwitchButtonListener(Sgl::Button* button) : Sgl::ActionListener<Sgl::Button>(button) {}

            virtual void onAction(Sgl::ActionEvent* event) override
            {
                Sml::Color foreground = Editor::getInstance().getForeground();
                Sml::Color background = Editor::getInstance().getBackground();

                LOG_APP_INFO("Switching foreground (0x%08" PRIx32 ") with background (0x%08" PRIx32 ")",
                             foreground,
                             background);

                Editor::getInstance().setForeground(background);
                Editor::getInstance().setBackground(foreground);
            }
        };

        m_SwitchButton->setOnAction(new SwitchButtonListener(m_SwitchButton));
    }

    virtual void dispose() override
    {
        m_Widget->removeChild(m_BackgroundRect);
        m_Widget->removeChild(m_ForegroundRect);
        m_Widget->removeChild(m_SwitchButton);

        delete m_BackgroundRect;
        delete m_ForegroundRect;
        delete m_SwitchButton;

        if (m_ColorPickerMenu != nullptr)
        {
            delete m_ColorPickerMenu;
        }
    }

    virtual void prerenderControl() override
    {
        m_BackgroundRect->setFillColor(Editor::getInstance().getBackground());
        m_ForegroundRect->setFillColor(Editor::getInstance().getForeground());
    }

    virtual const Sgl::Control* getControl() const override { return m_Widget; }
    virtual Sgl::Control* getModifiableControl() override { return m_Widget; }

    virtual int32_t computePrefHeight(int32_t width) const override { return COLOR_PICKER_PREF_SIZE; }
    virtual int32_t computePrefWidth(int32_t height) const override { return COLOR_PICKER_PREF_SIZE; }

    virtual void layoutChildren() override
    {
        Sml::Rectangle<int32_t> contentArea = m_Widget->getContentArea();

        int32_t squareSize = 4 * std::min(contentArea.width, contentArea.height) / 7;

        m_BackgroundRect->setLayoutX(contentArea.pos.x + contentArea.width  - squareSize);
        m_BackgroundRect->setLayoutY(contentArea.pos.y + contentArea.height - squareSize);
        m_BackgroundRect->setLayoutWidth(squareSize);
        m_BackgroundRect->setLayoutHeight(squareSize);

        m_ForegroundRect->setLayoutX(contentArea.pos.x);
        m_ForegroundRect->setLayoutY(contentArea.pos.y);
        m_ForegroundRect->setLayoutWidth(squareSize);
        m_ForegroundRect->setLayoutHeight(squareSize);

        // m_SwitchButton->setLayoutWidth(squareSize / 2);
        // m_SwitchButton->setLayoutHeight(squareSize / 2);
        m_SwitchButton->setLayoutWidth(m_SwitchButton->computePrefWidth());
        m_SwitchButton->setLayoutHeight(m_SwitchButton->computePrefHeight());
        m_SwitchButton->setLayoutX(contentArea.pos.x + contentArea.width - m_SwitchButton->getLayoutWidth());
        m_SwitchButton->setLayoutY(contentArea.pos.y);
    }

private:
    BackgroundForegroundSwitcher* m_Widget         = nullptr;

    Sgl::Rectangle*               m_BackgroundRect  = nullptr;
    Sgl::Rectangle*               m_ForegroundRect  = nullptr;
    Sgl::ContextMenu*             m_ColorPickerMenu = nullptr;
    ColorPicker*                  m_ColorPicker     = nullptr;
    Type                          m_BeingPicked;

    Sgl::ColorFill                m_BackgroundFill;
    Sgl::ColorFill                m_ForegroundFill;

    Sgl::Button*                  m_SwitchButton    = nullptr;
};

BackgroundForegroundSwitcher::BackgroundForegroundSwitcher()
{
    m_DefaultSkin = new BackgroundForegroundSwitcherSkin(this);
    setSkin(m_DefaultSkin);
}