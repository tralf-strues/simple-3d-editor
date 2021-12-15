/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file widget_impl.h
 * @date 2021-12-14
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "sgl/controls.h"
#include "sgl/shapes.h"
#include "sgl/containers.h"
#include "../gui/color_picker.h"
#include "texture_impl.h"

namespace plugin
{
    template <typename C>
    class ComponentWrapper
    {
    public:
        ComponentWrapper(C* component) : m_Component(component) { assert(component); }
        virtual ~ComponentWrapper() { delete m_Component; }

        C* GetComponent() { return m_Component; }
        

    private:
        C* m_Component = nullptr;
    };

    class ButtonImpl : public IButton, public ComponentWrapper<Sgl::Button>
    {
    public:
        ButtonImpl(Sgl::Button* button);
        virtual ~ButtonImpl() = default;

        virtual int32_t GetSizeX() override;
        virtual int32_t GetSizeY() override;

        virtual void SetClickCallback(IClickCallback* callback) override;
    };

    class SliderImpl : public ISlider, public ComponentWrapper<Sgl::Slider>
    {
    public:
        SliderImpl(Sgl::Slider* slider);
        virtual ~SliderImpl() = default;

        virtual int32_t GetSizeX() override;
        virtual int32_t GetSizeY() override;

        virtual void SetSliderCallback(ISliderCallback* callback) override;

        virtual float GetValue() override;
        virtual void SetValue(float value) override;
    };

    class LabelImpl : public ILabel, public ComponentWrapper<Sgl::Text>
    {
    public:
        LabelImpl(Sgl::Text* text);
        virtual ~LabelImpl() = default;

        virtual int32_t GetSizeX() override;
        virtual int32_t GetSizeY() override;

        virtual void SetText(const char* text) override;
    };

    class IconImpl : public IIcon, public ComponentWrapper<Sgl::ImageView>
    {
    public:
        IconImpl(Sgl::ImageView* imageView);
        virtual ~IconImpl() = default;

        virtual int32_t GetSizeX() override;
        virtual int32_t GetSizeY() override;

        virtual void SetIcon(const ITexture* icon) override;
    };

    class PaletteImpl : public IPalette, public ComponentWrapper<Paint::ColorPicker>
    {
    public:
        PaletteImpl(Paint::ColorPicker* colorPicker);
        virtual ~PaletteImpl() = default;

        virtual int32_t GetSizeX() override;
        virtual int32_t GetSizeY() override;

        virtual void SetPaletteCallback(IPaletteCallback* callback) override;
    };

    class PreferencesPanelImpl : public IPreferencesPanel, public ComponentWrapper<Sgl::Container>
    {
    public:
        PreferencesPanelImpl(Sgl::Container* container);
        virtual ~PreferencesPanelImpl() = default;

        virtual int32_t GetSizeX() override;
        virtual int32_t GetSizeY() override;

        virtual void Attach(IButton*  button,  int32_t x, int32_t y) override;
        virtual void Attach(ILabel*   label,   int32_t x, int32_t y) override;
        virtual void Attach(ISlider*  slider,  int32_t x, int32_t y) override;
        virtual void Attach(IIcon*    icon,    int32_t x, int32_t y) override;
        virtual void Attach(IPalette* palette, int32_t x, int32_t y) override;
    };

    class WidgetFactoryImpl : public IWidgetFactory
    {
    public:
        virtual ~WidgetFactoryImpl() = default;

        virtual IButton* CreateDefaultButtonWithIcon(const char* icon_file_name) override;
        virtual IButton* CreateDefaultButtonWithText(const char* text) override;
        virtual IButton* CreateButtonWithIcon(int32_t size_x, int32_t size_y, const char* icon_file_name) override;
        virtual IButton* CreateButtonWithText(int32_t size_x, int32_t size_y, const char* text, int32_t char_size) override;

        virtual ISlider* CreateDefaultSlider(float range_min, float range_max) override;
        virtual ISlider* CreateSlider(int32_t size_x, int32_t size_y, float range_min, float range_max) override;

        virtual ILabel*  CreateDefaultLabel(const char* text) override;
        virtual ILabel*  CreateLabel(int32_t size_x, int32_t size_y, const char* text, int32_t char_size) override;

        virtual IIcon*   CreateIcon(int32_t size_x, int32_t size_y) override;

        virtual IPalette* CreatePalette() override;

        virtual IPreferencesPanel* CreatePreferencesPanel() override;
    };
};