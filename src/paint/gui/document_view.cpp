/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file document_view.cpp
 * @date 2021-12-06
 * 
 * @copyright Copyright (c) 2021
 */

#include "paint/gui/document_view.h"
#include "paint/paint_editor.h"

using namespace Paint;

//------------------------------------------------------------------------------
// DocumentView
//------------------------------------------------------------------------------
class ActiveDocumentFilter : public Sgl::FocusListener<InnerWindow>
{
public:
    ActiveDocumentFilter(DocumentView* view) : Sgl::FocusListener<InnerWindow>(view->getView()), m_View(view) {}

    virtual void onFocusReceived(Sgl::FocusReceivedEvent*)
    {
        Paint::Editor::getInstance().setActiveDocument(m_View->getDocument());
    }

private:
    DocumentView* m_View;
};

const Sgl::ShadowSpecification DocumentView::SHADOW = {{0, 3}, {1.1, 1}, 3, 0x22'22'22'18};

DocumentView::DocumentView(Document* document, Sgl::Scene* scene)
    : m_Document(document),
      m_View(new InnerWindow(document->getName().c_str(), scene)),
    //   m_ScrollPane(new Sgl::ScrollPane()),
      m_Canvas(new Canvas(document)),
      m_Title(document->getName())
{
    m_View->updateTitle(m_Title.c_str());
    // m_ScrollPane->setContent(m_Canvas);
    // m_View->addChild(m_ScrollPane);
    m_View->addChild(m_Canvas);

    m_View->getEventDispatcher()->attachFilter(ActiveDocumentFilter::EVENT_TYPES, new ActiveDocumentFilter(this));

    // m_View->setShadow(&SHADOW); TODO: Add shadows only after optimizing them!
}

Document* DocumentView::getDocument() { return m_Document; }
InnerWindow* DocumentView::getView() { return m_View; }

//------------------------------------------------------------------------------
// Canvas
//------------------------------------------------------------------------------
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
        LOG_APP_INFO("CanvasMousePressListener");

        getComponent()->requestDrag();
    }
};

class CanvasDragListener : public Sgl::DragListener<Canvas>
{
public:
    CanvasDragListener(Canvas* canvas) : Sgl::DragListener<Canvas>(canvas) {}

    virtual void onDragStart(Sgl::DragStartEvent* event)
    {
        Sml::Vec2i localPos = getComponent()->computeSceneToLocalPos({event->getX(), event->getY()});
        m_CurX = localPos.x - getComponent()->getTexturePos().x;
        m_CurY = localPos.y - getComponent()->getTexturePos().y;
     
        LOG_APP_INFO("CanvasDragListener::onDragStart(canvasPos = {%d, %d})", m_CurX, m_CurY);

        Sml::Renderer& renderer = Sml::Renderer::getInstance();
        renderer.pushTarget();
        renderer.setTarget(getComponent()->getDocument()->getActiveLayer()->getTexture());

        Editor::getInstance().getActiveTool()->onActionStart(Sml::Vec2i(m_CurX, m_CurY));

        renderer.popTarget();
    }

    virtual void onDragMove(Sgl::DragMoveEvent* event)
    {
        int32_t newX = m_CurX + event->getDeltaX();
        int32_t newY = m_CurY + event->getDeltaY();

        LOG_APP_INFO("CanvasDragListener::onDragMove(canvasPos = {%d, %d})", newX, newY);

        Sml::Renderer& renderer = Sml::Renderer::getInstance();
        renderer.pushTarget();
        renderer.setTarget(getComponent()->getDocument()->getActiveLayer()->getTexture());

        Editor::getInstance().getActiveTool()->onAction(Sml::Vec2i(newX, newY),
                                                        Sml::Vec2i(event->getDeltaX(), event->getDeltaY()));

        renderer.popTarget();

        m_CurX = newX;
        m_CurY = newY;
    }

    virtual void onDragEnd(Sgl::DragEndEvent* event)
    {
        Sml::Vec2i localPos = getComponent()->computeSceneToLocalPos({event->getX(), event->getY()});
        m_CurX = localPos.x - getComponent()->getTexturePos().x;
        m_CurY = localPos.y - getComponent()->getTexturePos().y;

        LOG_APP_INFO("CanvasDragListener::onDragEnd(canvasPos = {%d, %d})", m_CurX, m_CurY);

        Sml::Renderer& renderer = Sml::Renderer::getInstance();
        renderer.pushTarget();
        renderer.setTarget(getComponent()->getDocument()->getActiveLayer()->getTexture());

        Editor::getInstance().getActiveTool()->onActionEnd(Sml::Vec2i(m_CurX, m_CurY));

        renderer.popTarget();
    }

private:
    int32_t m_CurX = 0;
    int32_t m_CurY = 0;
};

Canvas::Canvas(Document* document) : m_Document(document)
{
    assert(document);

    setBackground(&BACKGROUND);
    getEventDispatcher()->attachHandler(CanvasMousePressListener::EVENT_TYPES, new CanvasMousePressListener(this));
    getEventDispatcher()->attachHandler(CanvasDragListener::EVENT_TYPES, new CanvasDragListener(this));
}

Document* Canvas::getDocument() { return m_Document; }
void Canvas::setDocument(Document* texture) { assert(texture); m_Document = texture; }

void Canvas::prerenderSelf()
{
    Container::prerenderSelf();

    m_Document->applyLayersToCanvas();
    Sml::renderTexture(*m_Document->getCanvas(), getTexturePos());
}

int32_t Canvas::computeCustomPrefWidth(int32_t height) const
{
    return m_Document->getWidth();
}

int32_t Canvas::computeCustomPrefHeight(int32_t width) const
{
    return m_Document->getHeight();
}

Sml::Vec2i Canvas::getTexturePos() const
{
    return Sml::Vec2i((getLayoutWidth()  - m_Document->getWidth())  / 2,
                      (getLayoutHeight() - m_Document->getHeight()) / 2);
}