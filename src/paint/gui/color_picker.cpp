/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file color_picker.cpp
 * @date 2021-12-08
 * 
 * @copyright Copyright (c) 2021
 */

#include <algorithm>
#include "sgl/scene/style/default_skins.h"
#include "paint/gui/color_picker.h"

using namespace Paint;

//------------------------------------------------------------------------------
// ColorPicker listeners
//------------------------------------------------------------------------------
class GradientPressListener : public Sml::Listener
{
public:
    DEFINE_STATIC_LISTENED_EVENT_TYPES(Sml::MouseButtonPressedEvent::getStaticType())

public:
    GradientPressListener(ColorPickerSkin* skin) : m_Skin(skin) {}

    virtual void onEvent(Sml::Event* event) override
    {
        m_Skin->getModifiableControl()->requestDrag();

        event->consume();
    }

private:
    ColorPickerSkin* m_Skin = nullptr;
};

class ColorPickerDragListener : public Sgl::DragListener<ColorPicker>
{
public:
    ColorPickerDragListener(ColorPicker* colorPicker) : DragListener<ColorPicker>(colorPicker) {}

    virtual void onDragStart(Sgl::DragStartEvent* event)
    {
        LOG_LIB_INFO("ColorPickerDragListener::onDragStart()");

        ColorPicker&          colorPicker = *getComponent();
        ColorPickerSkin&      skin        = dynamic_cast<ColorPickerSkin&>(*colorPicker.getSkin());
        const Sgl::Rectangle* svRect      = skin.getSaturationValueRect();

        Sml::Vec2i localPos = svRect->computeSceneToLocalPos({event->getX(), event->getY()});

        if (Sml::isPointInsideRectangle(localPos, svRect->getOriginBounds()))
        {
            Sml::ColorHsv newColor = colorPicker.getColorHsv();
            newColor.s = static_cast<float>(localPos.x) / svRect->getLayoutWidth();
            newColor.v = static_cast<float>(svRect->getLayoutHeight() - localPos.y) / svRect->getLayoutHeight();

            newColor.s = std::min(std::max(0.0f, newColor.s), 1.0f);
            newColor.v = std::min(std::max(0.0f, newColor.v), 1.0f);

            colorPicker.setColorHsv(newColor);
        }
    }

    virtual void onDragMove(Sgl::DragMoveEvent* event)
    {
        LOG_LIB_INFO("ColorPickerDragListener::onDragMove()");

        ColorPicker&          colorPicker = *getComponent();
        ColorPickerSkin&      skin        = dynamic_cast<ColorPickerSkin&>(*colorPicker.getSkin());
        const Sgl::Rectangle* svRect      = skin.getSaturationValueRect();

        Sml::Vec2i    localPos = svRect->computeSceneToLocalPos({event->getX(), event->getY()});
        Sml::ColorHsv newColor = colorPicker.getColorHsv();

        if (Sml::isPointInsideRectangle(localPos, svRect->getOriginBounds()))
        {
            newColor.s = static_cast<float>(localPos.x) / svRect->getLayoutWidth();
            newColor.v = static_cast<float>(svRect->getLayoutHeight() - localPos.y) / svRect->getLayoutHeight();

            newColor.s = std::min(std::max(0.0f, newColor.s), 1.0f);
            newColor.v = std::min(std::max(0.0f, newColor.v), 1.0f);
        }
        else
        {
            newColor.s += static_cast<float>(event->getDeltaX()) / svRect->getLayoutWidth();
            newColor.v += -1 * static_cast<float>(event->getDeltaY()) / svRect->getLayoutHeight();

            newColor.s = std::min(std::max(0.0f, newColor.s), 1.0f);
            newColor.v = std::min(std::max(0.0f, newColor.v), 1.0f);
        }

        colorPicker.setColorHsv(newColor);
    }
};

class HueSliderListener : public Sml::PropertyChangeListener<float>
{
public:
    HueSliderListener(ColorPicker* colorPicker) : m_ColorPicker(colorPicker) {}

    virtual void onPropertyChange(Sml::PropertyChangeEvent<float>* event) override
    {
        Sml::ColorHsv newColor = m_ColorPicker->getColorHsv();
        newColor.h = event->getNewValue();

        m_ColorPicker->setColorHsv(newColor);
    }

private:
    ColorPicker* m_ColorPicker = nullptr;
};

class AlphaSliderListener : public Sml::PropertyChangeListener<float>
{
public:
    AlphaSliderListener(ColorPicker* colorPicker) : m_ColorPicker(colorPicker) {}

    virtual void onPropertyChange(Sml::PropertyChangeEvent<float>* event) override
    {
        Sml::ColorHsv newColor = m_ColorPicker->getColorHsv();
        newColor.a = event->getNewValue();

        m_ColorPicker->setColorHsv(newColor);
    }

private:
    ColorPicker* m_ColorPicker = nullptr;
};

//------------------------------------------------------------------------------
// ColorPickerSkin
//------------------------------------------------------------------------------
// const int32_t     ROOT_VBOX_SPACING                = 10;
const Sgl::Insets COLOR_HBOX_PADDING               = Sgl::Insets(10);
const int32_t     COLOR_HBOX_SPACING               = 10;
const int32_t     SLIDERS_VBOX_SPACING             = 5;

const int32_t     SATURATION_VALUE_BOX_PREF_WIDTH  = 300;
const int32_t     SATURATION_VALUE_BOX_PREF_HEIGHT = 200;
const int32_t     FINAL_COLOR_CIRCLE_PREF_SIZE     = SATURATION_VALUE_BOX_PREF_HEIGHT / 8;
const int32_t     SLIDERS_PREF_WIDTH               = SATURATION_VALUE_BOX_PREF_WIDTH / 2;

const Sgl::LinearGradientFill GRAY_GRADIENT = Sgl::LinearGradientFill(Sgl::LinearGradientFill::Direction::VERTICAL,
                                                                      {{0.00, 0x00'00'00'00},
                                                                       {1.00, 0x00'00'00'FF}});

const Sgl::Border              POINTER_BORDER = Sgl::Border(1, Sml::COLOR_WHITE);
const int32_t                  POINTER_RADIUS = 6;
// const Sgl::ShadowSpecification FINAL_COLOR_SHADOW = Sgl::ShadowSpecification({0, 0}, {1.05, 1.05}, 3, 0x44'44'44'88);

ColorPickerSkin::ColorPickerSkin(ColorPicker* colorPicker)
    : m_ColorPicker(colorPicker),
      m_RootVBox(new Sgl::VBox()),
      m_SaturationValueBox(new Sgl::BlankContainer()),
      m_ColorHBox(new Sgl::HBox()),
      m_SlidersVBox(new Sgl::VBox()),
      m_SaturationValueRect(new Sgl::Rectangle()),
      m_Pointer(new Sgl::Circle(POINTER_RADIUS, &POINTER_BORDER)),
      m_FinalColorCircle(new Sgl::Circle(FINAL_COLOR_CIRCLE_PREF_SIZE / 2, nullptr)),
      m_HueSlider(new Sgl::Slider(new Sgl::DefaultSkins::SliderSkin(&Sgl::LinearGradientFill::RAINBOX_HORIZONTAL,
                                                                    nullptr, 0xEE'EE'EE'FF), 0, 360)),
      m_AlphaSlider(new Sgl::Slider(0, 1)),
      m_HexLabel(new Sgl::Text("HEX:"))
{
    assert(colorPicker);

    // m_RootVBox->addChildren(m_SaturationValueBox, m_ColorHBox, m_HexLabel);
    m_RootVBox->addChildren(m_SaturationValueBox, m_ColorHBox); // TODO: add m_HexLabel
    m_RootVBox->setFillAcross(true);
    // m_RootVBox->setSpacing(ROOT_VBOX_SPACING);

    m_SaturationValueBox->addChildren(m_SaturationValueRect, m_Pointer);
    m_SaturationValueBox->setPrefWidth(SATURATION_VALUE_BOX_PREF_WIDTH);
    m_SaturationValueBox->setPrefHeight(SATURATION_VALUE_BOX_PREF_HEIGHT);
    m_SaturationValueRect->setWidth(SATURATION_VALUE_BOX_PREF_WIDTH);
    m_SaturationValueRect->setHeight(SATURATION_VALUE_BOX_PREF_HEIGHT);

    m_ColorHBox->addChildren(m_FinalColorCircle, m_SlidersVBox);
    m_ColorHBox->setSpacing(COLOR_HBOX_SPACING);
    m_ColorHBox->setPadding(COLOR_HBOX_PADDING);
    m_ColorHBox->setGrowPriority(m_SlidersVBox, Sgl::BoxContainer::GrowPriority::ALWAYS);

    m_SlidersVBox->addChildren(m_HueSlider);
    m_SlidersVBox->addChildren(m_AlphaSlider);
    m_SlidersVBox->setSpacing(SLIDERS_VBOX_SPACING);
    m_SlidersVBox->setFillAcross(true);

    m_HueSlider->setPrefWidth(SLIDERS_PREF_WIDTH);
    m_AlphaSlider->setPrefWidth(SLIDERS_PREF_WIDTH);

    // m_FinalColorCircle->setShadow(&FINAL_COLOR_SHADOW);

    attach(colorPicker);
}

// Sml::Vec2i ColorPickerSkin::computePositionFromColor(Sml::Color color) const
// {
//     if (m_Gradient != nullptr)
//     {
//         const Sml::Color* pixels = m_Gradient->getBuffer();
//         size_t pixelsCount = m_Gradient->getTexture().getWidth() * m_Gradient->getTexture().getHeight();

//         for (size_t i = 0; i < pixelsCount; ++i)
//         {
//             if (pixels[i] == color)
//             {
//                 return Sml::Vec2i(i % m_Gradient->getTexture().getWidth(), i / m_Gradient->getTexture().getWidth());
//             }
//         }
//     }

//     return Sml::Vec2i(0, 0);
// }

// Sml::Color ColorPickerSkin::computeColorFromPosition(const Sml::Vec2i& pos) const
// {
//     if (m_Gradient != nullptr &&
//         pos.x < m_Gradient->getTexture().getWidth() &&
//         pos.y < m_Gradient->getTexture().getHeight())
//     {
//         return (*m_Gradient)[pos.y][pos.x];
//     }

//     return 0;
// }

void ColorPickerSkin::dispose()
{
    m_ColorPicker->removeChild(m_RootVBox);
    delete m_RootVBox;

    m_ColorPicker->getEventDispatcher()->detachHandler(m_ColorPickerDragListener);
    delete m_ColorPickerDragListener;
}

void ColorPickerSkin::attach(ColorPicker* colorPicker)
{
    assert(colorPicker);

    m_ColorPicker = colorPicker;
    m_ColorPicker->addChild(m_RootVBox);

    m_HueSlider->addOnPropertyChange(new HueSliderListener(m_ColorPicker));
    m_AlphaSlider->addOnPropertyChange(new AlphaSliderListener(m_ColorPicker));

    m_SaturationValueBox->getEventDispatcher()->attachHandler(GradientPressListener::EVENT_TYPES,
                                                              new GradientPressListener(this));

    m_ColorPickerDragListener = new ColorPickerDragListener(m_ColorPicker);
    m_ColorPicker->getEventDispatcher()->attachHandler(ColorPickerDragListener::EVENT_TYPES,
                                                       m_ColorPickerDragListener);
}

void ColorPickerSkin::prerenderControl()
{
    // if (m_Gradient == nullptr ||
    //     m_Gradient->getTexture().getWidth()  != m_ColorPicker->getLayoutWidth() ||
    //     m_Gradient->getTexture().getHeight() != m_ColorPicker->getLayoutHeight())
    // {
    //     if (m_Gradient == nullptr)
    //     {
    //         delete m_Gradient;
    //     }

    //     m_Gradient = new Sml::BufferedTexture(m_ColorPicker->getLayoutWidth(), m_ColorPicker->getLayoutHeight());

    //     Sml::Renderer::getInstance().pushTarget();
    //     Sml::Renderer::getInstance().setTarget(&m_Gradient->getTexture());

    //     Sml::Renderer::getInstance().setBlendMode(Sml::Renderer::BlendMode::BLEND);
    //     MAIN_GRADIENT.fillArea(m_ColorPicker->getOriginBounds(), m_ColorPicker->getOriginBounds());
    //     GRAY_GRADIENT.fillArea(m_ColorPicker->getOriginBounds(), m_ColorPicker->getOriginBounds());

    //     Sml::Renderer::getInstance().setBlendMode(Sml::Renderer::BlendMode::BLEND);
    //     Sml::Renderer::getInstance().popTarget();

    //     m_Gradient->updateBuffer();
    // }
    
    // m_Gradient->getTexture().copyTo(m_ColorPicker->getSnapshot(), nullptr, nullptr);

    static Sgl::LinearGradientFill s_SaturationValueFill = Sgl::LinearGradientFill({{0.00, 0xFF'FF'FF'FF},
                                                                                    {1.00, m_ColorPicker->getColor()}});
    static Sgl::Background s_SaturationValueBackground;

    Sml::ColorHsv baseColor = m_ColorPicker->getColorHsv();
    baseColor.s = 1;
    baseColor.v = 1;
    baseColor.a = 1;
    s_SaturationValueFill = Sgl::LinearGradientFill({{0.00, 0xFF'FF'FF'FF},
                                                     {1.00, Sml::convertHsvToRgb(baseColor)}});
    
    s_SaturationValueBackground.clearFills();
    s_SaturationValueBackground.addFill(&s_SaturationValueFill);
    s_SaturationValueBackground.addFill(&GRAY_GRADIENT);

    m_SaturationValueRect->setBackground(&s_SaturationValueBackground);

    m_FinalColorCircle->setFillColor(m_ColorPicker->getColor());

    m_HueSlider->setValue(m_ColorPicker->getColorHsv().h);
    m_AlphaSlider->setValue(m_ColorPicker->getColorHsv().a);
}

const Sgl::Control* ColorPickerSkin::getControl() const { return m_ColorPicker; }
Sgl::Control* ColorPickerSkin::getModifiableControl() { return m_ColorPicker; }

void ColorPickerSkin::layoutChildren()
{
    Sml::Rectangle<int32_t> contentArea = m_ColorPicker->getContentArea();

    m_RootVBox->setLayoutX(contentArea.pos.x);
    m_RootVBox->setLayoutY(contentArea.pos.y);
    m_RootVBox->setLayoutWidth(contentArea.width);
    m_RootVBox->setLayoutHeight(contentArea.height);

    // /* To avoid m_SaturationValueBox resizing when pointer gets out of the layout boundary */
    // m_Pointer->setLayoutX(0);
    // m_Pointer->setLayoutY(0);

    m_RootVBox->layout();
    m_ColorHBox->layout();
    m_SlidersVBox->layout();

    m_SaturationValueRect->setLayoutWidth(m_SaturationValueBox->getLayoutWidth());
    m_SaturationValueRect->setLayoutHeight(m_SaturationValueBox->getLayoutHeight());

    const Sml::Rectangle<int32_t> svRectRegion = m_SaturationValueRect->getLayoutBounds();
    const Sml::ColorHsv& color = m_ColorPicker->getColorHsv();
    Sml::Vec2i pointerPosition = svRectRegion.pos + Sml::Vec2i(svRectRegion.width * color.s,
                                                               svRectRegion.height * (1 - color.v));

    pointerPosition -= {m_Pointer->getRadius(), m_Pointer->getRadius()};

    // int32_t pointerRadius = m_Pointer->getRadius();
    // pointerPosition.x = std::min(std::max(svRectRegion.pos.x, pointerPosition.x),
    //                              svRectRegion.pos.x + svRectRegion.width - m_Pointer->getLayoutWidth());
    // pointerPosition.y = std::min(std::max(svRectRegion.pos.y, pointerPosition.y),
    //                              svRectRegion.pos.y + svRectRegion.height - m_Pointer->getLayoutHeight());

    m_Pointer->setLayoutX(pointerPosition.x);
    m_Pointer->setLayoutY(pointerPosition.y);
}

const Sgl::Rectangle* ColorPickerSkin::getSaturationValueRect() const { return m_SaturationValueRect; }

void ColorPickerSkin::updateSliders()
{
    Sml::ColorHsv hsv = m_ColorPicker->getColorHsv();
    m_HueSlider->setValue(hsv.h);
    m_AlphaSlider->setValue(hsv.a);
}

//------------------------------------------------------------------------------
// ColorPicker
//------------------------------------------------------------------------------
ColorPicker::ColorPicker()
{
    m_DefaultSkin = new ColorPickerSkin(this);
    setSkin(m_DefaultSkin);
}

Sml::ColorHsv ColorPicker::getColorHsv() const { return m_ColorHsv; }

void ColorPicker::setColorHsv(const Sml::ColorHsv& color)
{
    Sml::Color oldValue = m_ColorRgb;
    m_ColorRgb = Sml::convertHsvToRgb(color);
    m_ColorHsv = color;

    if (getOnPropertyChange() != nullptr && m_ColorRgb != oldValue)
    {
        getOnPropertyChange()->onPropertyChange(new Sml::PropertyChangeEvent<Sml::Color>(oldValue, m_ColorRgb));
    }
}

Sml::Color ColorPicker::getColor() const { return m_ColorRgb; }

void ColorPicker::setColor(Sml::Color color)
{
    Sml::Color oldValue = m_ColorRgb;
    m_ColorRgb = color;
    m_ColorHsv = Sml::convertRgbToHsv(m_ColorRgb);

    if (getOnPropertyChange() != nullptr && m_ColorRgb != oldValue)
    {
        getOnPropertyChange()->onPropertyChange(new Sml::PropertyChangeEvent<Sml::Color>(oldValue, m_ColorRgb));
    }
}

void ColorPicker::setOnPropertyChange(Sml::PropertyChangeListener<Sml::Color>* listener)
{
    m_PropertyChangeListener = listener;
}

Sml::PropertyChangeListener<Sml::Color>* ColorPicker::getOnPropertyChange() { return m_PropertyChangeListener; }