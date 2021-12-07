/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file document.cpp
 * @date 2021-12-06
 * 
 * @copyright Copyright (c) 2021
 */

#include "sml/sml_log.h"
#include "paint/document.h"

using namespace Paint;

Layer::Layer(size_t width, size_t height)
{
    assert(width  > 0);
    assert(height > 0);

    m_Texture = new Sml::Texture(width, height);
    
    Sml::Renderer::getInstance().pushTarget();
    Sml::Renderer::getInstance().setTarget(m_Texture);

    Sml::Renderer::getInstance().setColor(Sml::COLOR_TRANSPARENT);
    Sml::Renderer::getInstance().clear();

    Sml::Renderer::getInstance().popTarget();
}

Layer::~Layer()
{
    if (m_Texture != nullptr)
    {
        delete m_Texture;
    }
}

Sml::Texture* Layer::getTexture() { return m_Texture; }

Document::Document(const char* filename) : m_Name(filename)
{
    assert(filename);

    m_Canvas = new Sml::Texture;
    
    if (!m_Canvas->loadFromImage(filename))
    {
        LOG_APP_ERROR("File '%s' couldn't be found!", filename);
        assert(0);
    }

    addLayer(new Layer(m_Canvas->getWidth(), m_Canvas->getHeight()));
}

Document::Document(size_t width, size_t height, const char* name) : m_Name(name)
{
    assert(name);
    m_Canvas = new Sml::Texture(width, height);

    Sml::Renderer::getInstance().pushTarget();
    Sml::Renderer::getInstance().setTarget(m_Canvas);

    Sml::Renderer::getInstance().setColor(Sml::COLOR_WHITE);
    Sml::Renderer::getInstance().clear();

    Sml::Renderer::getInstance().popTarget();
    
    addLayer(new Layer(m_Canvas->getWidth(), m_Canvas->getHeight()));
}

Document::~Document()
{
    delete m_Canvas;

    for (auto layer : m_Layers)
    {
        delete layer;
    }
}

void Document::setName(const std::string& name) { m_Name = name;   }
const std::string& Document::getName() const    { return m_Name;   }
Sml::Texture* Document::getCanvas()             { return m_Canvas; }

size_t Document::getWidth() const  { assert(m_Canvas); return m_Canvas->getWidth();  }
size_t Document::getHeight() const { assert(m_Canvas); return m_Canvas->getHeight(); }

void Document::applyLayersToCanvas()
{
    for (auto layer : m_Layers)
    {
        layer->getTexture()->copyTo(m_Canvas, nullptr, nullptr);
    }
}

void Document::addLayer(Layer* layer)
{
    assert(layer);
    assert(layer->getTexture()->getWidth()  == getWidth());
    assert(layer->getTexture()->getHeight() == getHeight());

    m_Layers.push_back(layer);
}

void Document::removeLayer(Layer* layer)
{
    assert(layer);
    m_Layers.remove(layer);
}