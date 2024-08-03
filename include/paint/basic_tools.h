/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file basic_tools.h
 * @date 2021-12-06
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "tool.h"

namespace Paint
{
    class Brush : public Tool
    {
    public:
        virtual const char* getName() const override;
        virtual const char* getIconFilename() const override;

        virtual Sgl::Container* getPreferencesPanel() override;

        virtual void onAction(const Sml::Vec2i& pos, const Sml::Vec2i& displacement) override;

        int32_t getThickness() const;
        void setThickness(int32_t thickness);

    private:
        int32_t m_Thickness = 10;
    };

    class Eraser : public Tool
    {
    public:
        virtual const char* getName() const override;
        virtual const char* getIconFilename() const override;

        virtual Sgl::Container* getPreferencesPanel() override;

        virtual void onAction(const Sml::Vec2i& pos, const Sml::Vec2i& displacement) override;

        int32_t getThickness() const;
        void setThickness(int32_t thickness);

    private:
        int32_t m_Thickness = 10;
    };

    class RectangleTool : public Tool
    {
    public:
        virtual const char* getName() const override;
        virtual const char* getIconFilename() const override;

        virtual Sgl::Container* getPreferencesPanel() override;

        virtual void onActionStart(const Sml::Vec2i& pos) override;
        virtual void onAction(const Sml::Vec2i& pos, const Sml::Vec2i& displacement) override;
        virtual void onActionEnd(const Sml::Vec2i& pos) override;

        int32_t getThickness() const;
        void setThickness(int32_t thickness);

    private:
        Sml::Texture* m_OriginalTexture = nullptr;
        int32_t       m_Thickness       = 1;
        Sml::Vec2i    m_Origin          = {0, 0};
    };
};