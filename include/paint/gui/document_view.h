/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file document_view.h
 * @date 2021-12-06
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "sgl/scene/controls/scroll_pane.h"
#include "../../inner_window.h"
#include "../document.h"

namespace Paint
{
    class Canvas;

    class DocumentView
    {
    public:
        static const Sgl::ShadowSpecification SHADOW;

    public:
        DocumentView(Document* document, Sgl::Scene* scene);

        InnerWindow* getView();

    private:
        Document*        m_Document   = nullptr;
        InnerWindow*     m_View       = nullptr;
        Sgl::ScrollPane* m_ScrollPane = nullptr;
        Canvas*          m_Canvas     = nullptr;
        std::string      m_Title;
    };

    class Canvas : public Sgl::Container
    {
    public:
        static const Sgl::ColorFill  BACKGROUND_FILL;
        static const Sgl::Background BACKGROUND;

    public:
        Canvas(Document* document);
        virtual ~Canvas() override = default;

        Document* getDocument();
        void setDocument(Document* document);

        Sml::Vec2i getTexturePos() const;

    private:
        Document* m_Document = nullptr;

        virtual void prerenderSelf() override;

        virtual int32_t computeCustomPrefWidth(int32_t height = -1) const override;
        virtual int32_t computeCustomPrefHeight(int32_t width = -1) const override;
    };
};