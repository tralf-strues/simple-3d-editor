/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file explosion_brush.cpp
 * @date 2021-12-14
 * 
 * @copyright Copyright (c) 2021
 */

#include <assert.h>
#include <stdio.h>
#include "plugin_api.h"

const int   SEQUENCE_SIZE               = 5;
const char* FILENAME_EXPLOSION_SEQUENCE = "explosion/sequence_%d.png";
const char* FILENAME_EXPLOSION_SINGLE   = "explosion/single.png";
const int   ITERATION_DISTANCE_LENGTH   = 16;

plugin::IAPI* g_API = nullptr;

class ExplosionBrush : public plugin::ITool
{
public:
    ExplosionBrush()
    {
        static char filename[1024];
        for (int  i = 0; i < SEQUENCE_SIZE; ++i)
        {
            snprintf(filename, sizeof(filename), FILENAME_EXPLOSION_SEQUENCE, i);
            m_TexturesExplosionSequence[i] = g_API->GetTextureFactory()->CreateTexture(filename);
        }

        m_TextureExplosionSingle = g_API->GetTextureFactory()->CreateTexture(FILENAME_EXPLOSION_SINGLE);
    }

    virtual ~ExplosionBrush() override
    {
        delete m_TextureExplosionSingle;

        for (int i = 0; i < SEQUENCE_SIZE; ++i)
        {
            delete m_TexturesExplosionSequence[i];
        }
    }

    void Iterate()
    {
        m_Counter = (m_Counter + 1 >= SEQUENCE_SIZE) ? m_Counter = SEQUENCE_SIZE - 1 : m_Counter + 1;
    }

    bool ShouldIterate(int x, int y)
    {
        int deltaX = x - m_LastDrawX;
        int deltaY = y - m_LastDrawY;

        return deltaX * deltaX + deltaY * deltaY >= ITERATION_DISTANCE_LENGTH * ITERATION_DISTANCE_LENGTH;
    }

    void DrawTexture(plugin::ITexture* canvas, int x, int y, int counter)
    {
        plugin::ITexture* source = (counter == -1) ? m_TextureExplosionSingle : m_TexturesExplosionSequence[counter];
        canvas->CopyTexture(source, x - source->GetSizeX() / 2, y - source->GetSizeY() / 2);

        m_LastDrawX = x;
        m_LastDrawY = y;
    }

    virtual void ActionBegin(plugin::ITexture* canvas, int x, int y) override
    {
        assert(canvas);

        m_Counter = -1;

        /* To make it far away :) */
        m_LastDrawX = -100;
        m_LastDrawY = -100;
    }

    virtual void Action(plugin::ITexture* canvas, int x, int y, int dx, int dy) override
    {
        assert(canvas);

        if (ShouldIterate(x, y))
        {
            Iterate();
            DrawTexture(canvas, x, y, m_Counter);
        }
    }

    virtual void ActionEnd(plugin::ITexture* canvas, int x, int y) override
    {
        assert(canvas);

        if (m_Counter != -1)
        {
            Iterate();
            DrawTexture(canvas, x, y, m_Counter);
        }
        
        if (ShouldIterate(x, y))
        {
            DrawTexture(canvas, x, y, m_Counter);
        }
    }

    virtual const char* GetIconFileName() const override
    {
        return "explosion/icon.png";
    }

    virtual const char* GetName() const override
    {
        return "Explosion";
    }

    virtual plugin::IPreferencesPanel* GetPreferencesPanel() const override
    {
        return g_API->GetWidgetFactory()->CreatePreferencesPanel();
    }

private:
    plugin::ITexture* m_TexturesExplosionSequence[SEQUENCE_SIZE];

    plugin::ITexture* m_TextureExplosionSingle = nullptr;
    int               m_Counter                = 0;

    int               m_LastDrawX              = 0;
    int               m_LastDrawY              = 0;
};

class ExplosionPlugin : public plugin::IPlugin
{
public:
    virtual plugin::Filters GetFilters() const override
    {
        return {nullptr, 0};
    }

    virtual plugin::Tools GetTools() const override
    {
        plugin::Tools tools = {};
        tools.count = 1;
        tools.tools = new plugin::ITool*[tools.count];
        tools.tools[0] = new ExplosionBrush();

        return tools;
    }
};

extern "C" plugin::IPlugin* Create(plugin::IAPI* api)
{
    g_API = api;
    return new ExplosionPlugin();
}

extern "C" void Destroy(plugin::IPlugin* plugin)
{
    delete plugin;
}

extern "C" uint32_t Version()
{
    return plugin::kVersion;
}