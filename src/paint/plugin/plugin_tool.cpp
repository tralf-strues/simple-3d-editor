/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file plugin_tool.cpp
 * @date 2021-12-15
 * 
 * @copyright Copyright (c) 2021
 */

#include "paint/plugin/plugin_tool.h"
#include "paint/plugin/api_impl.h"
#include "paint/paint_editor.h"

using namespace Paint;

PluginTool::PluginTool(plugin::ITool* tool) : m_PluginTool(tool)
{
    m_IconFilename = new char[512];
    snprintf(m_IconFilename, 512, "plugins/%s", m_PluginTool->GetIconFileName());
}

PluginTool::~PluginTool() { delete m_PluginTool; delete m_IconFilename; }

const char* PluginTool::getName() const { return m_PluginTool->GetName(); }

const char* PluginTool::getIconFilename() const
{
    return m_IconFilename;
}

Sgl::Container* PluginTool::getPreferencesPanel()
{
    return dynamic_cast<plugin::PreferencesPanelImpl*>(m_PluginTool->GetPreferencesPanel())->GetComponent();
}

void PluginTool::onActionStart(const Sml::Vec2i& pos)
{
    plugin::TextureImpl texture(Sml::Renderer::getInstance().getTarget());
    m_PluginTool->ActionBegin(&texture, pos.x, pos.y);
}

void PluginTool::onAction(const Sml::Vec2i& pos, const Sml::Vec2i& displacement)
{
    plugin::TextureImpl texture(Sml::Renderer::getInstance().getTarget());
    m_PluginTool->Action(&texture, pos.x, pos.y, displacement.x, displacement.y);
}

void PluginTool::onActionEnd(const Sml::Vec2i& pos)
{
    plugin::TextureImpl texture(Sml::Renderer::getInstance().getTarget());
    m_PluginTool->ActionEnd(&texture, pos.x, pos.y);
}