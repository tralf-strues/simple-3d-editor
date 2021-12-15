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
const int   DEFAULT_THICKNESS           = 24;

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

        int scaledWidth  = source->GetSizeX() * (static_cast<float>(m_Thickness) / DEFAULT_THICKNESS);
        int scaledHeight = source->GetSizeY() * (static_cast<float>(m_Thickness) / DEFAULT_THICKNESS);

        canvas->CopyTexture(source, x - scaledWidth / 2, y - scaledHeight / 2, scaledWidth, scaledHeight);

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

    virtual plugin::IPreferencesPanel* GetPreferencesPanel() override
    {
        plugin::IPreferencesPanel* panel = g_API->GetWidgetFactory()->CreatePreferencesPanel();
        
        plugin::ILabel*  label       = g_API->GetWidgetFactory()->CreateDefaultLabel("Thickness");
        plugin::ILabel*  sliderLabel = g_API->GetWidgetFactory()->CreateDefaultLabel("0");
        plugin::ISlider* slider      = g_API->GetWidgetFactory()->CreateSlider(75, 0, 4, 100);

        class SliderCallback : public plugin::ISliderCallback
        {
        public:
            SliderCallback(plugin::ILabel* sliderLabel, int* thickness)
                : m_SliderLabel(sliderLabel), m_Thickness(thickness) {}

            virtual void RespondOnSlide(float old_value, float current_value) override
            {
                printf("RespondOnSlide(old_value = %lg, current_value = %lg)\n", old_value, current_value);

                *m_Thickness = current_value;

                snprintf(m_Str, sizeof(m_Str), "%02.0f", current_value);
                m_SliderLabel->SetText(m_Str);
            };

        private:
            plugin::ILabel* m_SliderLabel;
            int*            m_Thickness;
            char            m_Str[128];
        };

        slider->SetSliderCallback(new SliderCallback(sliderLabel, &m_Thickness));

        panel->Attach(label, 5, 5);
        panel->Attach(slider, 5, 25);
        panel->Attach(sliderLabel, 85, 25);

        return panel;
    }

private:
    plugin::ITexture* m_TexturesExplosionSequence[SEQUENCE_SIZE];

    plugin::ITexture* m_TextureExplosionSingle = nullptr;
    int               m_Counter                = 0;

    int               m_LastDrawX              = 0;
    int               m_LastDrawY              = 0;

    int               m_Thickness              = DEFAULT_THICKNESS;
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
    delete g_API;
}

extern "C" uint32_t Version()
{
    return plugin::kVersion;
}