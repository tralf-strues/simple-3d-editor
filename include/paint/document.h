/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file document.h
 * @date 2021-12-06
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include <string>
#include <list>
#include "sml/sml_graphics_wrapper.h"

namespace Paint
{
    class Layer
    {
    public:
        Layer(size_t width, size_t height);
        ~Layer();

        Sml::Texture* getTexture();

    private:
        Sml::Texture* m_Texture = nullptr;
    };

    class Document
    {
    public:
        Document(const char* filename);
        Document(size_t width, size_t height, const char* name = "untitled");
        ~Document();

        void setName(const std::string& name);
        const std::string& getName() const;
        Sml::Texture* getCanvas();

        size_t getWidth() const;
        size_t getHeight() const;

        void applyLayersToCanvas();

        void addLayer(Layer* layer);
        void removeLayer(Layer* layer);

        Layer* getActiveLayer();
        void setActiveLayer(Layer* layer);

    private:
        std::string       m_Name;
        Sml::Texture*     m_Canvas      = nullptr;
        Layer*            m_ActiveLayer = nullptr;
        std::list<Layer*> m_Layers;
    };
};