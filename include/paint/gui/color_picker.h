/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file color_picker.h
 * @date 2021-12-08
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "sgl/scene/controls/control.h"
#include "sgl/scene/shapes/rectangle.h"
#include "sgl/scene/shapes/circle.h"

namespace Paint
{
    class ColorPicker : public Sgl::Control
    {
    public:
        ColorPicker();

        Sml::Color getColor() const;
        void setColor(Sml::Color color);

        void setOnPropertyChange(Sml::PropertyChangeListener<Sml::Color>* listener);
        Sml::PropertyChangeListener<Sml::Color>* getOnPropertyChange(); // TODO: get rid of

    private:
        Sml::Color m_Color = Sml::COLOR_WHITE;

        Sml::PropertyChangeListener<Sml::Color>* m_PropertyChangeListener = nullptr;
    };

    class ColorPickerSkin : public Sgl::BaseSkin<ColorPicker>
    {
    public:
        ColorPickerSkin(ColorPicker* colorPicker);

        Sml::Vec2i computePositionFromColor(Sml::Color color) const;
        Sml::Color computeColorFromPosition(const Sml::Vec2i& pos) const;

        virtual void dispose() override;
        virtual void attach(ColorPicker* colorPicker) override;

        virtual Sgl::Component* getHitComponent(int32_t x, int32_t y) override;
        virtual void prerenderControl() override;

        virtual const Sgl::Control* getControl() const override;
        virtual Sgl::Control* getModifiableControl() override;

        virtual int32_t computePrefWidth(int32_t height = -1) const override;
        virtual int32_t computePrefHeight(int32_t width = -1) const override;

        virtual void layoutChildren() override;

    private:
        ColorPicker*          m_ColorPicker = nullptr;

        Sml::BufferedTexture* m_Gradient    = nullptr;
        Sgl::Rectangle*       m_Rectangle   = nullptr;
        Sgl::Circle*          m_Pointer     = nullptr;
    };
};