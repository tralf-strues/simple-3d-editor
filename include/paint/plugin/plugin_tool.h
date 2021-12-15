/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file plugin_tool.h
 * @date 2021-12-15
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "../tool.h"
#include "plugin_api.h"

namespace Paint
{
    class PluginTool : public Paint::Tool
    {
    public:
        PluginTool(plugin::ITool* tool);

        virtual ~PluginTool() override;

        virtual const char* getName() const override;
        virtual const char* getIconFilename() const override;
        virtual Sgl::Container* getPreferencesPanel() override;

        virtual void onActionStart(const Sml::Vec2i& pos) override;
        virtual void onAction(const Sml::Vec2i& pos, const Sml::Vec2i& displacement) override;
        virtual void onActionEnd(const Sml::Vec2i& pos) override;

    private:
        plugin::ITool* m_PluginTool   = nullptr;
        char*          m_IconFilename = nullptr;
    };
}