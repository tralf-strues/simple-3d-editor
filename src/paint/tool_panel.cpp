/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file tool_panel.cpp
 * @date 2021-12-06
 * 
 * @copyright Copyright (c) 2021
 */

#include "resource_manager.h"
#include "paint/tool_panel.h"
#include "paint/paint_editor.h"
#include "sgl/scene/controls/button.h"

using namespace Paint;

ToolPanel::ToolPanel() : m_View(new Sgl::VBox()), m_Tools(new Sgl::TilePane())
{
    m_View->addChild(m_Tools);
}

ToolPanel::~ToolPanel()
{
    delete m_View;
    delete m_Tools;
}

Sgl::VBox* ToolPanel::getView()
{
    return m_View;
}

void ToolPanel::loadTools()
{
    for (auto tool : Editor::getInstance().getTools())
    {
        LOG_APP_INFO("Loading tool to ToolPanel {name = '%s', icon = '%s'}", tool->getName(), tool->getIconFilename());
        m_Tools->addChild(new Sgl::Button(RESOURCE_IMAGE(tool->getIconFilename())));
    }
}