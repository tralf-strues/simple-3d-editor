/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file color_picker.cpp
 * @date 2021-12-08
 * 
 * @copyright Copyright (c) 2021
 */

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

class GradientPointerDrag : public Sgl::DragListener<ColorPicker>
{
public:
    GradientPointerDrag(ColorPicker* colorPicker) : DragListener<ColorPicker>(colorPicker) {}

    void updateColor(const Sml::Vec2i& newPos)
    {
        ColorPicker&     colorPicker = *getComponent();
        ColorPickerSkin& skin        = dynamic_cast<ColorPickerSkin&>(*colorPicker.getSkin());

        Sml::Vec2i localPos = colorPicker.computeSceneToLocalPos(newPos);

        if (Sml::isPointInsideRectangle(localPos, colorPicker.getContentArea()))
        {
            colorPicker.setColor(skin.computeColorFromPosition(localPos));
        }
    }

    virtual void onDragStart(Sgl::DragStartEvent* event)
    {
        LOG_LIB_INFO("GradientPointerDrag::onDragStart() called");

        updateColor(Sml::Vec2i(event->getX(), event->getY()));
    }

    virtual void onDragMove(Sgl::DragMoveEvent* event)
    {
        LOG_LIB_INFO("GradientPointerDrag::onDragMove() called");

        updateColor(Sml::Vec2i(event->getX(), event->getY()));
    }
};

//------------------------------------------------------------------------------
// ColorPickerSkin
//------------------------------------------------------------------------------
const int32_t PREF_WIDTH  = 300;
const int32_t PREF_HEIGHT = 200;

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

const Sgl::Border              POINTER_BORDER = Sgl::Border(1, Sml::COLOR_WHITE);
const int32_t                  POINTER_RADIUS = 6;
const Sgl::ShadowSpecification POINTER_SHADOW = Sgl::ShadowSpecification({0, 0}, {1.1, 1.1}, 2, 0x00'00'00'FF);

ColorPickerSkin::ColorPickerSkin(ColorPicker* colorPicker)
    : m_ColorPicker(colorPicker),
      m_Rectangle(new Sgl::Rectangle()),
      m_Pointer(new Sgl::Circle(POINTER_RADIUS, &POINTER_BORDER))
{
    assert(colorPicker);

    m_Pointer->setShadow(&POINTER_SHADOW);

    attach(colorPicker);
}

Sml::Vec2i ColorPickerSkin::computePositionFromColor(Sml::Color color) const
{
    if (m_Gradient != nullptr)
    {
        const Sml::Color* pixels = m_Gradient->getBuffer();
        size_t pixelsCount = m_Gradient->getTexture().getWidth() * m_Gradient->getTexture().getHeight();

        for (size_t i = 0; i < pixelsCount; ++i)
        {
            if (pixels[i] == color)
            {
                return Sml::Vec2i(i % m_Gradient->getTexture().getWidth(), i / m_Gradient->getTexture().getWidth());
            }
        }
    }

    return Sml::Vec2i(0, 0);
}

Sml::Color ColorPickerSkin::computeColorFromPosition(const Sml::Vec2i& pos) const
{
    if (m_Gradient != nullptr &&
        pos.x < m_Gradient->getTexture().getWidth() &&
        pos.y < m_Gradient->getTexture().getHeight())
    {
        return (*m_Gradient)[pos.y][pos.x];
    }

    return 0;
}

void ColorPickerSkin::dispose()
{
    m_ColorPicker->removeChild(m_Rectangle);
    m_ColorPicker->removeChild(m_Pointer);

    delete m_Rectangle;
    delete m_Pointer;
}

void ColorPickerSkin::attach(ColorPicker* colorPicker)
{
    assert(colorPicker);

    m_ColorPicker = colorPicker;

    m_ColorPicker->addChild(m_Rectangle);
    m_ColorPicker->addChild(m_Pointer);

    m_Rectangle->getEventDispatcher()->attachHandler(GradientPressListener::EVENT_TYPES,
                                                     new GradientPressListener(this));

    m_ColorPicker->getEventDispatcher()->attachHandler(GradientPointerDrag::EVENT_TYPES,
                                                       new GradientPointerDrag(m_ColorPicker));
}

Sgl::Component* ColorPickerSkin::getHitComponent(int32_t x, int32_t y)
{
    // int32_t radius = m_Pointer->getRadius();

    // if (Sml::lengthSquare(Sml::Vec2i(x - radius, y - radius) - m_Pointer->getLayoutPos()) <= radius * radius)
    // {
    //     return m_Pointer;
    // }

    if (Sml::isPointInsideRectangle(Sml::Vec2i(x, y), m_Rectangle->getLayoutBounds()))
    {
        return m_Rectangle;
    }

    return nullptr;
}

void ColorPickerSkin::prerenderControl()
{
    if (m_Gradient == nullptr ||
        m_Gradient->getTexture().getWidth()  != m_ColorPicker->getLayoutWidth() ||
        m_Gradient->getTexture().getHeight() != m_ColorPicker->getLayoutHeight())
    {
        if (m_Gradient == nullptr)
        {
            delete m_Gradient;
        }

        m_Gradient = new Sml::BufferedTexture(m_ColorPicker->getLayoutWidth(), m_ColorPicker->getLayoutHeight());

        Sml::Renderer::getInstance().pushTarget();
        Sml::Renderer::getInstance().setTarget(&m_Gradient->getTexture());

        Sml::Renderer::getInstance().setBlendMode(Sml::Renderer::BlendMode::BLEND);
        MAIN_GRADIENT.fillArea(m_ColorPicker->getOriginBounds(), m_ColorPicker->getOriginBounds());
        GRAY_GRADIENT.fillArea(m_ColorPicker->getOriginBounds(), m_ColorPicker->getOriginBounds());

        Sml::Renderer::getInstance().setBlendMode(Sml::Renderer::BlendMode::BLEND);
        Sml::Renderer::getInstance().popTarget();

        m_Gradient->updateBuffer();
    }
    
    m_Gradient->getTexture().copyTo(m_ColorPicker->getSnapshot(), nullptr, nullptr);
}

const Sgl::Control* ColorPickerSkin::getControl() const { return m_ColorPicker; }
Sgl::Control* ColorPickerSkin::getModifiableControl() { return m_ColorPicker; }

int32_t ColorPickerSkin::computePrefWidth(int32_t height) const { return PREF_WIDTH;  }
int32_t ColorPickerSkin::computePrefHeight(int32_t width) const { return PREF_HEIGHT; }

void ColorPickerSkin::layoutChildren()
{
    Sml::Rectangle<int32_t> contentArea = m_ColorPicker->getContentArea();

    m_Rectangle->setLayoutX(contentArea.pos.x);
    m_Rectangle->setLayoutY(contentArea.pos.y);
    m_Rectangle->setLayoutWidth(contentArea.width);
    m_Rectangle->setLayoutHeight(contentArea.height);

    Sml::Vec2i pointerPosition = computePositionFromColor(m_ColorPicker->getColor());
    m_Pointer->setLayoutWidth(m_Pointer->computePrefWidth());
    m_Pointer->setLayoutHeight(m_Pointer->computePrefHeight());
    m_Pointer->setLayoutX(contentArea.pos.x + pointerPosition.x);
    m_Pointer->setLayoutY(contentArea.pos.y + pointerPosition.y);
}

//------------------------------------------------------------------------------
// ColorPicker
//------------------------------------------------------------------------------
ColorPicker::ColorPicker()
{
    m_DefaultSkin = new ColorPickerSkin(this);
    setSkin(m_DefaultSkin);
}

Sml::Color ColorPicker::getColor() const { return m_Color; }
void ColorPicker::setColor(Sml::Color color)
{
    Sml::Color oldValue = m_Color;
    m_Color = color;

    if (getOnPropertyChange() != nullptr && m_Color != oldValue)
    {
        getOnPropertyChange()->onPropertyChange(new Sml::PropertyChangeEvent<Sml::Color>(oldValue, m_Color));
    }
}

void ColorPicker::setOnPropertyChange(Sml::PropertyChangeListener<Sml::Color>* listener)
{
    m_PropertyChangeListener = listener;
}

Sml::PropertyChangeListener<Sml::Color>* ColorPicker::getOnPropertyChange() { return m_PropertyChangeListener; }