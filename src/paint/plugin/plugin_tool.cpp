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

void PluginTool::onActionStart(const Sml::Vec2i& pos)
{
    plugin::TextureImpl texture(Editor::getInstance().getActiveDocument()->getActiveLayer()->getTexture());
    m_PluginTool->ActionBegin(&texture, pos.x, pos.y);
}

void PluginTool::onAction(const Sml::Vec2i& pos, const Sml::Vec2i& displacement)
{
    plugin::TextureImpl texture(Editor::getInstance().getActiveDocument()->getActiveLayer()->getTexture());
    m_PluginTool->Action(&texture, pos.x, pos.y, displacement.x, displacement.y);
}

void PluginTool::onActionEnd(const Sml::Vec2i& pos)
{
    plugin::TextureImpl texture(Editor::getInstance().getActiveDocument()->getActiveLayer()->getTexture());
    m_PluginTool->ActionEnd(&texture, pos.x, pos.y);
}