/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file paint_editor.cpp
 * @date 2021-12-06
 * 
 * @copyright Copyright (c) 2021
 */

#include "paint/paint_editor.h"

using namespace Paint;

Editor* Editor::s_Instance = nullptr;

void Editor::init()
{
    s_Instance = new Editor();
}

bool Editor::isInitialized()
{
    return s_Instance != nullptr;
}

Editor& Editor::getInstance()
{
    return *s_Instance;
}

Editor::~Editor()
{
    for (auto tool : m_Tools)
    {
        delete tool;
    }
}

Tool* Editor::getActiveTool()
{
    return m_ActiveTool;
}

void Editor::setActiveTool(Tool* tool)
{
    m_ActiveTool = tool;
}

const std::list<Tool*>& Editor::getTools() const
{
    return m_Tools;
}

void Editor::addTool(Tool* tool)
{
    assert(tool);
    m_Tools.push_back(tool);
}

const std::list<Filter*>& Editor::getFilters() const
{
    return m_Filters;
}

void Editor::addFilter(Filter* filter)
{
    assert(filter);
    m_Filters.push_back(filter);
}

void Editor::applyFilter(Filter* filter)
{
    assert(filter);

    if (getActiveDocument() != nullptr)
    {
        LOG_APP_INFO("Applying '%s' filter.", filter->getName());

        Sml::Renderer::getInstance().pushSetTarget(getActiveDocument()->getActiveLayer()->getTexture());

        filter->apply();

        Sml::Renderer::getInstance().popTarget();
    }
}

Document* Editor::getActiveDocument()
{
    return m_ActiveDocument;
}

void Editor::setActiveDocument(Document* document)
{
    assert(document);
    m_ActiveDocument = document;
}

const std::list<Document*>& Editor::getDocuments() const
{
    return m_Documents;
}

void Editor::addDocument(Document* document)
{
    assert(document);
    m_Documents.push_back(document);
}

// const std::list<plugin::IPlugin*>& Editor::getPlugins() const
// {
//     return m_Plugins;
// }

// void Editor::addPlugin(plugin::IPlugin* plugin)
// {
//     assert(plugin);
//     m_Plugins.push_back(plugin);
// }

Sml::Color Editor::getBackground() const { return m_Background; }
void Editor::setBackground(Sml::Color background) { m_Background = background; }

Sml::Color Editor::getForeground() const { return m_Foreground; }
void Editor::setForeground(Sml::Color foreground) { m_Foreground = foreground; }