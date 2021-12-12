/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file color_picker.h
 * @date 2021-12-08
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "sgl/scene/containers/box_container.h"
#include "sgl/scene/controls/slider.h"
#include "sgl/scene/shapes/rectangle.h"
#include "sgl/scene/shapes/circle.h"
#include "sgl/scene/shapes/text.h"

class ColorPickerDragListener;

namespace Paint
{
    class ColorPicker : public Sgl::Control
    {
    public:
        ColorPicker();

        Sml::ColorHsv getColorHsv() const;
        void setColorHsv(const Sml::ColorHsv& color);

        Sml::Color getColor() const;
        void setColor(Sml::Color color);

        void setOnPropertyChange(Sml::PropertyChangeListener<Sml::Color>* listener);
        Sml::PropertyChangeListener<Sml::Color>* getOnPropertyChange(); // TODO: get rid of

    private:
        Sml::Color    m_ColorRgb = 0x00'00'00'FF;
        Sml::ColorHsv m_ColorHsv = {0, 0, 0, 1};

        Sml::PropertyChangeListener<Sml::Color>* m_PropertyChangeListener = nullptr;
    };

    class ColorPickerSkin : public Sgl::BaseSkin<ColorPicker>
    {
    public:
        ColorPickerSkin(ColorPicker* colorPicker);

        // Sml::Vec2i computePositionFromColor(Sml::Color color) const;
        // Sml::Color computeColorFromPosition(const Sml::Vec2i& pos) const;

        virtual void dispose() override;
        virtual void attach(ColorPicker* colorPicker) override;

        virtual void prerenderControl() override;

        virtual const Sgl::Control* getControl() const override;
        virtual Sgl::Control* getModifiableControl() override;

        // virtual int32_t computePrefWidth(int32_t height = -1) const override;
        // virtual int32_t computePrefHeight(int32_t width = -1) const override;

        virtual void layoutChildren() override;

        const Sgl::Rectangle* getSaturationValueRect() const;
        void updateSliders();

    private:
        ColorPicker*          m_ColorPicker         = nullptr;

        Sgl::VBox*            m_RootVBox            = nullptr;
        Sgl::BlankContainer*  m_SaturationValueBox  = nullptr;
        Sgl::HBox*            m_ColorHBox           = nullptr;
        Sgl::VBox*            m_SlidersVBox         = nullptr;
        
        Sgl::Rectangle*       m_SaturationValueRect = nullptr;
        Sgl::Circle*          m_Pointer             = nullptr;

        Sgl::Circle*          m_FinalColorCircle    = nullptr;
        Sgl::Slider*          m_HueSlider           = nullptr;
        Sgl::Slider*          m_AlphaSlider         = nullptr;
        Sgl::Text*            m_HexLabel            = nullptr;

        ColorPickerDragListener* m_ColorPickerDragListener = nullptr;

        // Sml::BufferedTexture* m_Gradient        = nullptr;
        // Sgl::Rectangle*       m_Rectangle       = nullptr;
    };
};