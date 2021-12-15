/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file widget_impl.cpp
 * @date 2021-12-14
 * 
 * @copyright Copyright (c) 2021
 */

#include "paint/plugin/widget_impl.h"

using namespace plugin;

//------------------------------------------------------------------------------
// ButtonImpl
//------------------------------------------------------------------------------
ButtonImpl::ButtonImpl(Sgl::Button* button) : ComponentWrapper<Sgl::Button>(button) {}

int32_t ButtonImpl::GetSizeX() { return GetComponent()->computePrefWidth();  }
int32_t ButtonImpl::GetSizeY() { return GetComponent()->computePrefHeight(); }

void ButtonImpl::SetClickCallback(IClickCallback* callback)
{
    assert(callback);

    class ActionHandler : public Sgl::ActionListener<Sgl::Button>
    {
    public:
        ActionHandler(Sgl::Button* button, IClickCallback* callback)
            : Sgl::ActionListener<Sgl::Button>(button), m_Callback(callback) {}

        virtual void onAction(Sgl::ActionEvent*) override
        {
            m_Callback->RespondOnClick();
        }

    private:
        IClickCallback* m_Callback;
    };

    GetComponent()->setOnAction(new ActionHandler(GetComponent(), callback));
}

//------------------------------------------------------------------------------
// SliderImpl
//------------------------------------------------------------------------------
SliderImpl::SliderImpl(Sgl::Slider* slider) : ComponentWrapper<Sgl::Slider>(slider) {}

int32_t SliderImpl::GetSizeX() { return GetComponent()->computePrefWidth();  }
int32_t SliderImpl::GetSizeY() { return GetComponent()->computePrefHeight(); }

void SliderImpl::SetSliderCallback(ISliderCallback* callback)
{
    assert(callback);

    class PropertyChangeHandler : public Sml::PropertyChangeListener<float>
    {
    public:
        PropertyChangeHandler(ISliderCallback* callback) : m_Callback(callback) {}

        virtual void onPropertyChange(Sml::PropertyChangeEvent<float>* event) override
        {
            m_Callback->RespondOnSlide(event->getOldValue(), event->getNewValue());
        }

    private:
        ISliderCallback* m_Callback;
    };

    GetComponent()->addOnPropertyChange(new PropertyChangeHandler(callback));
}

float SliderImpl::GetValue() { return GetComponent()->getValue(); }
void SliderImpl::SetValue(float value) { GetComponent()->setValue(value); }

//------------------------------------------------------------------------------
// LabelImpl
//------------------------------------------------------------------------------
LabelImpl::LabelImpl(Sgl::Text* text) : ComponentWrapper<Sgl::Text>(text) {}

int32_t LabelImpl::GetSizeX() { return GetComponent()->computePrefWidth(); }
int32_t LabelImpl::GetSizeY() { return GetComponent()->computePrefHeight(); }

void LabelImpl::SetText(const char* text) { GetComponent()->setString(text); }

//------------------------------------------------------------------------------
// IconImpl
//------------------------------------------------------------------------------
IconImpl::IconImpl(Sgl::ImageView* imageView) : ComponentWrapper<Sgl::ImageView>(imageView) {}

int32_t IconImpl::GetSizeX() { return GetComponent()->computePrefWidth(); }
int32_t IconImpl::GetSizeY() { return GetComponent()->computePrefHeight(); }

void IconImpl::SetIcon(const ITexture* icon)
{
    GetComponent()->setImage(new Sgl::Image((dynamic_cast<const TextureImpl*>(icon))->GetTexture()));
}

//------------------------------------------------------------------------------
// PaletteImpl
//------------------------------------------------------------------------------
PaletteImpl::PaletteImpl(Paint::ColorPicker* colorPicker) : ComponentWrapper<Paint::ColorPicker>(colorPicker) {}

int32_t PaletteImpl::GetSizeX() { return GetComponent()->computePrefWidth();  }
int32_t PaletteImpl::GetSizeY() { return GetComponent()->computePrefHeight(); }

void PaletteImpl::SetPaletteCallback(IPaletteCallback* callback)
{
    assert(callback);

    class PropertyChangeHandler : public Sml::PropertyChangeListener<Sml::Color>
    {
    public:
        PropertyChangeHandler(IPaletteCallback* callback) : m_Callback(callback) {}

        virtual void onPropertyChange(Sml::PropertyChangeEvent<Sml::Color>* event) override
        {
            m_Callback->RespondOnChangeColor(event->getNewValue());
        }

    private:
        IPaletteCallback* m_Callback;
    };

    GetComponent()->setOnPropertyChange(new PropertyChangeHandler(callback));
}

//------------------------------------------------------------------------------
// PreferencesPanelImpl
//------------------------------------------------------------------------------
PreferencesPanelImpl::PreferencesPanelImpl(Sgl::Container* container) : ComponentWrapper<Sgl::Container>(container) {}

int32_t PreferencesPanelImpl::GetSizeX() { return GetComponent()->computePrefWidth();  }
int32_t PreferencesPanelImpl::GetSizeY() { return GetComponent()->computePrefHeight(); }

void PreferencesPanelImpl::Attach(IButton* button, int32_t x, int32_t y)
{
    Sgl::Button* component = dynamic_cast<ButtonImpl*>(button)->GetComponent();
    component->setLayoutX(x);
    component->setLayoutY(y);
    GetComponent()->addChild(component);
}

void PreferencesPanelImpl::Attach(ILabel* label, int32_t x, int32_t y)
{
    Sgl::Text* component = dynamic_cast<LabelImpl*>(label)->GetComponent();
    component->setLayoutX(x);
    component->setLayoutY(y);
    GetComponent()->addChild(component);
}

void PreferencesPanelImpl::Attach(ISlider* slider, int32_t x, int32_t y)
{
    Sgl::Slider* component = dynamic_cast<SliderImpl*>(slider)->GetComponent();
    component->setLayoutX(x);
    component->setLayoutY(y);
    GetComponent()->addChild(component);
}

void PreferencesPanelImpl::Attach(IIcon* icon, int32_t x, int32_t y)
{
    Sgl::ImageView* component = dynamic_cast<IconImpl*>(icon)->GetComponent();
    component->setLayoutX(x);
    component->setLayoutY(y);
    GetComponent()->addChild(component);
}

void PreferencesPanelImpl::Attach(IPalette* palette, int32_t x, int32_t y)
{
    Paint::ColorPicker* component = dynamic_cast<PaletteImpl*>(palette)->GetComponent();
    component->setLayoutX(x);
    component->setLayoutY(y);
    GetComponent()->addChild(component);
}

//------------------------------------------------------------------------------
// WidgetFactoryImpl
//------------------------------------------------------------------------------
IButton* WidgetFactoryImpl::CreateDefaultButtonWithIcon(const char* icon_file_name)
{
    return new ButtonImpl(new Sgl::Button(new Sgl::Image(icon_file_name, Sgl::ImageFormat::PNG)));
}

IButton* WidgetFactoryImpl::CreateDefaultButtonWithText(const char* text)
{
    return new ButtonImpl(new Sgl::Button(text));
}

IButton* WidgetFactoryImpl::CreateButtonWithIcon(int32_t size_x, int32_t size_y, const char* icon_file_name)
{
    Sgl::Button* button = new Sgl::Button(new Sgl::Image(icon_file_name, Sgl::ImageFormat::PNG));
    button->setPrefWidth(size_x);
    button->setPrefHeight(size_y);

    return new ButtonImpl(button);
}

IButton* WidgetFactoryImpl::CreateButtonWithText(int32_t size_x, int32_t size_y, const char* text, int32_t char_size)
{
    Sgl::Button* button = new Sgl::Button(text);
    button->setPrefWidth(size_x);
    button->setPrefHeight(size_y);

    return new ButtonImpl(button);
}

ISlider* WidgetFactoryImpl::CreateDefaultSlider(float range_min, float range_max)
{
    return new SliderImpl(new Sgl::Slider(range_min, range_max));
}

ISlider* WidgetFactoryImpl::CreateSlider(int32_t size_x, int32_t size_y, float range_min, float range_max)
{
    Sgl::Slider* slider = new Sgl::Slider(range_min, range_max);
    slider->setPrefWidth(size_x);

    return new SliderImpl(slider);
}

ILabel* WidgetFactoryImpl::CreateDefaultLabel(const char* text)
{
    return new LabelImpl(new Sgl::Text(text));
}

ILabel* WidgetFactoryImpl::CreateLabel(int32_t size_x, int32_t size_y, const char* text, int32_t char_size)
{
    return CreateDefaultLabel(text);
}

IIcon* WidgetFactoryImpl::CreateIcon(int32_t size_x, int32_t size_y)
{
    return new IconImpl(new Sgl::ImageView());
}

IPalette* WidgetFactoryImpl::CreatePalette()
{
    return new PaletteImpl(new Paint::ColorPicker());
}

IPreferencesPanel* WidgetFactoryImpl::CreatePreferencesPanel()
{
    return new PreferencesPanelImpl(new Sgl::Container());
}