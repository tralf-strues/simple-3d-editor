/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file color_picker_widget.cpp
 * @date 2021-12-07
 * 
 * @copyright Copyright (c) 2021
 */

#include "sgl/scene/shapes/rectangle.h"
#include "sgl/scene/controls/button.h"
#include "paint/gui/color_picker_widget.h"
#include "paint/paint_editor.h"
#include "resource_manager.h"

using namespace Paint;

static const char*   ICON_SWITCH_BUTTON     = "icons/curved_arrow.png";
static const int32_t COLOR_PICKER_PREF_SIZE = 38;

class ColorPickerWidgetSkin : public Sgl::BaseSkin<ColorPickerWidget>
{
public:
    ColorPickerWidgetSkin(ColorPickerWidget* widget) : m_Widget(widget) { attach(m_Widget); }
    
    virtual void attach(ColorPickerWidget* widget) override
    {
        assert(widget);

        m_Widget = widget;

        m_Widget->addChild(m_BackgroundRect = new Sgl::Rectangle());
        m_Widget->addChild(m_ForegroundRect = new Sgl::Rectangle());

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
    }

    virtual void prerenderControl() override
    {
        m_BackgroundFill = Sgl::ColorFill(Editor::getInstance().getBackground());
        m_BackgroundRect->setFill(&m_BackgroundFill);

        m_ForegroundFill = Sgl::ColorFill(Editor::getInstance().getForeground());
        m_ForegroundRect->setFill(&m_ForegroundFill);
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
    ColorPickerWidget* m_Widget         = nullptr;

    Sgl::Rectangle*    m_BackgroundRect = nullptr;
    Sgl::Rectangle*    m_ForegroundRect = nullptr;
    Sgl::Button*       m_SwitchButton   = nullptr;

    Sgl::ColorFill     m_BackgroundFill;
    Sgl::ColorFill     m_ForegroundFill;
};

ColorPickerWidget::ColorPickerWidget()
{
    m_DefaultSkin = new ColorPickerWidgetSkin(this);
    setSkin(m_DefaultSkin);
}