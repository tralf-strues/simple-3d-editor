/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file canvas.cpp
 * @date 2021-11-15
 * 
 * @copyright Copyright (c) 2021
 */

#include "sml/sml_log.h"
#include "canvas.h"

const Sgl::ColorFill  Canvas::BACKGROUND_FILL = {0xA3'A3'A3'FF};
const Sgl::Background Canvas::BACKGROUND      = {&BACKGROUND_FILL};

class CanvasMousePressListener : public Sgl::ComponentEventListener<Canvas>
{
public:
    DEFINE_STATIC_LISTENED_EVENT_TYPES(Sml::MouseButtonPressedEvent::getStaticType())

public:
    CanvasMousePressListener(Canvas* canvas) : Sgl::ComponentEventListener<Canvas>(canvas) {}

    virtual void onEvent(Sml::Event* event) override
    {
        LOG_APP_INFO("CanvasMousePressListener called");

        getComponent()->requestDrag();
    }
};

class CanvasDragListener : public Sgl::DragListener<Canvas>
{
public:
    CanvasDragListener(Canvas* canvas) : Sgl::DragListener<Canvas>(canvas) {}

    virtual void onDragStart(Sgl::DragStartEvent* event)
    {
        LOG_APP_INFO("CanvasDragListener::onDragStart() called");

        Sml::Vec2<int32_t> localPos = getComponent()->computeSceneToLocalPos({event->getX(), event->getY()});
        m_CurX = localPos.x - getComponent()->getTexturePos().x;
        m_CurY = localPos.y - getComponent()->getTexturePos().y;
    }

    virtual void onDragMove(Sgl::DragMoveEvent* event)
    {
        LOG_APP_INFO("CanvasDragListener::onDragMove() called");

        int32_t newX = m_CurX + event->getDeltaX();
        int32_t newY = m_CurY + event->getDeltaY();

        Sml::Renderer& renderer = Sml::Renderer::getInstance();

        renderer.pushTarget();
        renderer.setTarget(getComponent()->getTexture());

        renderer.setColor(Sml::COLOR_BLACK);
        Sml::renderLine({m_CurX, m_CurY}, {newX, newY});
        
        renderer.popTarget();

        m_CurX = newX;
        m_CurY = newY;
    }

private:
    int32_t m_CurX = 0;
    int32_t m_CurY = 0;
};

Canvas::Canvas(int32_t width, int32_t height)
{
    assert(width > 0);
    assert(height > 0);

    m_Texture = new Sml::Texture(width, height);

    Sml::Renderer& renderer = Sml::Renderer::getInstance();

    renderer.pushTarget();
    renderer.setTarget(m_Texture);

    renderer.setColor(Sml::COLOR_WHITE);
    renderer.clear();

    renderer.popTarget();

    setBackground(&BACKGROUND);

    getEventDispatcher()->attachHandler(CanvasMousePressListener::EVENT_TYPES,
                                        new CanvasMousePressListener(this));
    
    getEventDispatcher()->attachHandler(CanvasDragListener::EVENT_TYPES, new CanvasDragListener(this));
}

Canvas::~Canvas()
{
    if (m_Texture != nullptr)
    {
        delete m_Texture;
    }
}

Sml::Texture* Canvas::getTexture()
{
    return m_Texture;
}

void Canvas::prerenderSelf()
{
    if (m_Texture == nullptr) { return; }

    Container::prerenderSelf();

    Sml::renderTexture(*m_Texture, getTexturePos());
}

int32_t Canvas::computeCustomPrefWidth(int32_t height) const
{
    return m_Texture->getWidth();
}

int32_t Canvas::computeCustomPrefHeight(int32_t width) const
{
    return m_Texture->getHeight();
}

Sml::Vec2<int32_t> Canvas::getTexturePos() const
{
    Sml::Vec2<int32_t> pos = getLayoutPos();
    pos.x = (getLayoutWidth()  - m_Texture->getWidth()) / 2;
    pos.y = (getLayoutHeight() - m_Texture->getHeight()) / 2;

    return pos;
}