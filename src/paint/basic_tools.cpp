/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file basic_tools.cpp
 * @date 2021-12-06
 * 
 * @copyright Copyright (c) 2021
 */

#include "sgl/containers.h"
#include "sgl/controls.h"
#include "paint/basic_tools.h"
#include "paint/paint_editor.h"

using namespace Paint;

//------------------------------------------------------------------------------
// Brush
//------------------------------------------------------------------------------
const char* Brush::getName() const         { return "Brush"; }
const char* Brush::getIconFilename() const { return "icons/paintbrush.png"; }

void Brush::onAction(const Sml::Vec2i& pos, const Sml::Vec2i& displacement)
{
    Sml::Renderer& renderer = Sml::Renderer::getInstance();

    renderer.setColor(Editor::getInstance().getForeground());
    Sml::renderLine(pos - displacement, pos, m_Thickness);
}

int32_t Brush::getThickness() const         { return m_Thickness; }
void Brush::setThickness(int32_t thickness) { assert(thickness > 0); m_Thickness = thickness; }

Sgl::Container* Brush::getPreferencesPanel()
{
    Sgl::VBox* vbox = new Sgl::VBox();
    vbox->setBackground(nullptr);
    vbox->setSpacing(5);
    vbox->setPadding(Sgl::Insets(5));
    vbox->setFillAcross(true);
    vbox->setPrefWidth(125);

    vbox->addChild(new Sgl::Text("Thickness"));

    Sgl::SliderWithLabel* sliderWithLabel = new Sgl::SliderWithLabel(1, 100, "%02.0f");
    sliderWithLabel->getSlider()->setValue(m_Thickness);
    vbox->addChild(sliderWithLabel);

    class ThicknessSliderHandler : public Sml::PropertyChangeListener<float>
    {
    public:
        ThicknessSliderHandler(Brush* brush) : m_Brush(brush) {}

        virtual void onPropertyChange(Sml::PropertyChangeEvent<float>* event) override
        {
            m_Brush->setThickness(static_cast<int32_t>(event->getNewValue()));
        }

    private:
        Brush* m_Brush;
    };

    sliderWithLabel->getSlider()->addOnPropertyChange(new ThicknessSliderHandler(this));

    return vbox;
}

//------------------------------------------------------------------------------
// Eraser
//------------------------------------------------------------------------------
const char* Eraser::getName() const { return "Eraser"; }
const char* Eraser::getIconFilename() const { return "icons/eraser.png"; }

Sgl::Container* Eraser::getPreferencesPanel()
{
    Sgl::VBox* vbox = new Sgl::VBox();
    vbox->setBackground(nullptr);
    vbox->setSpacing(5);
    vbox->setPadding(Sgl::Insets(5));
    vbox->setFillAcross(true);
    vbox->setPrefWidth(125);

    vbox->addChild(new Sgl::Text("Thickness"));

    Sgl::SliderWithLabel* sliderWithLabel = new Sgl::SliderWithLabel(1, 100, "%02.0f");
    sliderWithLabel->getSlider()->setValue(m_Thickness);
    vbox->addChild(sliderWithLabel);

    class ThicknessSliderHandler : public Sml::PropertyChangeListener<float>
    {
    public:
        ThicknessSliderHandler(Eraser* eraser) : m_Eraser(eraser) {}

        virtual void onPropertyChange(Sml::PropertyChangeEvent<float>* event) override
        {
            m_Eraser->setThickness(static_cast<int32_t>(event->getNewValue()));
        }

    private:
        Eraser* m_Eraser;
    };

    sliderWithLabel->getSlider()->addOnPropertyChange(new ThicknessSliderHandler(this));

    return vbox;
}

void Eraser::onAction(const Sml::Vec2i& pos, const Sml::Vec2i& displacement)
{
    Sml::Renderer& renderer = Sml::Renderer::getInstance();

    renderer.setColor(Sml::COLOR_TRANSPARENT);
    renderer.setBlendMode(Sml::Renderer::BlendMode::NONE);

    Sml::renderLine(pos - displacement, pos, m_Thickness);

    renderer.setBlendMode(Sml::Renderer::BlendMode::BLEND);
}

int32_t Eraser::getThickness() const { return m_Thickness; }
void Eraser::setThickness(int32_t thickness) { m_Thickness = thickness; }

//------------------------------------------------------------------------------
// RectangleTool
//------------------------------------------------------------------------------
const char* RectangleTool::getName() const { return "Rectangle"; }
const char* RectangleTool::getIconFilename() const { return "icons/rectangle.png"; }

Sgl::Container* RectangleTool::getPreferencesPanel()
{
    Sgl::VBox* vbox = new Sgl::VBox();
    vbox->setBackground(nullptr);
    vbox->setSpacing(5);
    vbox->setPadding(Sgl::Insets(5));
    vbox->setFillAcross(true);
    vbox->setPrefWidth(125);

    vbox->addChild(new Sgl::Text("Thickness"));

    Sgl::SliderWithLabel* sliderWithLabel = new Sgl::SliderWithLabel(1, 100, "%02.0f");
    sliderWithLabel->getSlider()->setValue(m_Thickness);
    vbox->addChild(sliderWithLabel);

    class ThicknessSliderHandler : public Sml::PropertyChangeListener<float>
    {
    public:
        ThicknessSliderHandler(RectangleTool* rectangleTool) : m_RectangleTool(rectangleTool) {}

        virtual void onPropertyChange(Sml::PropertyChangeEvent<float>* event) override
        {
            m_RectangleTool->setThickness(static_cast<int32_t>(event->getNewValue()));
        }

    private:
        RectangleTool* m_RectangleTool;
    };

    sliderWithLabel->getSlider()->addOnPropertyChange(new ThicknessSliderHandler(this));

    return vbox;
}

void RectangleTool::onActionStart(const Sml::Vec2i& pos)
{
    Sml::Renderer& renderer = Sml::Renderer::getInstance();

    m_Origin          = pos;
    m_OriginalTexture = renderer.getTarget()->copy();
}

void RectangleTool::onAction(const Sml::Vec2i& pos, const Sml::Vec2i& displacement)
{
    Sml::Renderer& renderer = Sml::Renderer::getInstance();

    m_OriginalTexture->copyTo(renderer.getTarget(), nullptr, nullptr);

    Sml::Rectangle<int32_t> rectangle(std::min(pos.x, m_Origin.x),
                                      std::min(pos.y, m_Origin.y),
                                      std::abs(pos.x - m_Origin.x),
                                      std::abs(pos.y - m_Origin.y));

    renderer.setColor(Paint::Editor::getInstance().getBackground());
    Sml::renderFilledRect(rectangle);

    renderer.setColor(Paint::Editor::getInstance().getForeground());
    Sml::renderRect(rectangle, m_Thickness);
}

void RectangleTool::onActionEnd(const Sml::Vec2i& pos)
{
    Sml::Renderer& renderer = Sml::Renderer::getInstance();

}

int32_t RectangleTool::getThickness() const { return m_Thickness; }
void RectangleTool::setThickness(int32_t thickness) { m_Thickness = thickness; }