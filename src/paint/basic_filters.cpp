/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file basic_filters.cpp
 * @date 2021-12-15
 * 
 * @copyright Copyright (c) 2021
 */

#include "sgl/controls.h"
#include "paint/basic_filters.h"
#include "paint/paint_editor.h"

using namespace Paint;

SharpenFilter::~SharpenFilter() { delete m_BlurKernel; }

const char* SharpenFilter::getName() const { return "Sharpen"; }

Sgl::Container* SharpenFilter::getPreferencesPanel()
{
    Sgl::VBox* vbox = new Sgl::VBox();
    vbox->setBackground(nullptr);
    vbox->setSpacing(5);
    vbox->setPadding(Sgl::Insets(5));
    vbox->setFillAcross(true);
    vbox->setPrefWidth(125);

    /* Blur */
    vbox->addChild(new Sgl::Text("Blur radius"));

    Sgl::SliderWithLabel* sliderBlurRadius = new Sgl::SliderWithLabel(1, 20, "%02.0f");
    sliderBlurRadius->getSlider()->setValue(m_BlurKernel->getRadius());
    vbox->addChild(sliderBlurRadius);

    class BlurRadiusSliderHandler : public Sml::PropertyChangeListener<float>
    {
    public:
        BlurRadiusSliderHandler(SharpenFilter* filter) : m_Filter(filter) {}

        virtual void onPropertyChange(Sml::PropertyChangeEvent<float>* event) override
        {
            m_Filter->setBlurRadius(static_cast<int32_t>(event->getNewValue()));
            Paint::Editor::getInstance().applyFilter(m_Filter);
        }

    private:
        SharpenFilter* m_Filter;
    };

    sliderBlurRadius->getSlider()->addOnPropertyChange(new BlurRadiusSliderHandler(this));

    /* Amount */
    vbox->addChild(new Sgl::Text("Amount"));

    Sgl::SliderWithLabel* sliderAmount = new Sgl::SliderWithLabel(0.5, 5, "%02.0f");
    sliderAmount->getSlider()->setValue(m_Amount);
    vbox->addChild(sliderAmount);

    class AmountSliderHandler : public Sml::PropertyChangeListener<float>
    {
    public:
        AmountSliderHandler(SharpenFilter* filter) : m_Filter(filter) {}

        virtual void onPropertyChange(Sml::PropertyChangeEvent<float>* event) override
        {
            m_Filter->setAmount(event->getNewValue());
            Paint::Editor::getInstance().applyFilter(m_Filter);
        }

    private:
        SharpenFilter* m_Filter;
    };

    sliderAmount->getSlider()->addOnPropertyChange(new AmountSliderHandler(this));

    /* Cancel */
    class CancelButtonActionListener : public Sgl::ActionListener<Sgl::Button>
    {
    public:
        CancelButtonActionListener(Sgl::Button* button, SharpenFilter* filter)
            : Sgl::ActionListener<Sgl::Button>(button), m_Filter(filter) {}

        virtual void onAction(Sgl::ActionEvent* event) override
        {
            LOG_APP_INFO("Cancel");
            m_Filter->setCanceled(true);
            Paint::Editor::getInstance().applyFilter(m_Filter);
        }

    private:
        SharpenFilter* m_Filter = nullptr;
    };

    Sgl::Button* cancelButton = new Sgl::Button("Cancel");
    cancelButton->setOnAction(new CancelButtonActionListener(cancelButton, this));
    vbox->addChild(cancelButton);

    /* Apply */
    class ApplyButtonActionListener : public Sgl::ActionListener<Sgl::Button>
    {
    public:
        ApplyButtonActionListener(Sgl::Button* button, SharpenFilter* filter)
            : Sgl::ActionListener<Sgl::Button>(button), m_Filter(filter) {}

        virtual void onAction(Sgl::ActionEvent* event) override
        {
            LOG_APP_INFO("Apply");
            m_Filter->setCanceled(false);
            Paint::Editor::getInstance().applyFilter(m_Filter);
        }

    private:
        SharpenFilter* m_Filter = nullptr;
    };

    Sgl::Button* applyButton = new Sgl::Button("Apply");
    applyButton->setOnAction(new ApplyButtonActionListener(applyButton, this));
    vbox->addChild(applyButton);

    return vbox;
}

void SharpenFilter::init(Sml::Texture* texture)
{
    m_OriginalTexture = texture->copy();
}

void SharpenFilter::apply()
{
    Sml::Renderer& renderer = Sml::Renderer::getInstance();

    if (m_Canceled)
    {
        m_OriginalTexture->copyTo(renderer.getTarget(), nullptr, nullptr);
        m_Canceled = false;
        return;
    }

    int32_t width  = static_cast<int32_t>(m_OriginalTexture->getWidth());
    int32_t height = static_cast<int32_t>(m_OriginalTexture->getHeight());

    Sml::Color* originalImage = m_OriginalTexture->readPixels();
    Sml::Color* blurredImage  = new Sml::Color[width * height];

    Sml::applyKernel(m_BlurKernel, originalImage, blurredImage, width, height);

    for (int32_t y = 0; y < height; ++y)
    {
        for (int32_t x = 0; x < width; ++x)
        {
            Sml::Color originalColor = originalImage[x + y * width];
            Sml::Color blurredColor  = blurredImage[x + y * width];

            int32_t colorR = static_cast<int32_t>(Sml::colorGetR(originalColor)) - static_cast<int32_t>(Sml::colorGetR(blurredColor));
            int32_t colorG = static_cast<int32_t>(Sml::colorGetG(originalColor)) - static_cast<int32_t>(Sml::colorGetG(blurredColor));
            int32_t colorB = static_cast<int32_t>(Sml::colorGetB(originalColor)) - static_cast<int32_t>(Sml::colorGetB(blurredColor));
            int32_t colorA = static_cast<int32_t>(Sml::colorGetA(originalColor)) - static_cast<int32_t>(Sml::colorGetA(blurredColor));

            colorR = std::max(static_cast<int32_t>(colorR * m_Amount), 0);
            colorG = std::max(static_cast<int32_t>(colorG * m_Amount), 0);
            colorB = std::max(static_cast<int32_t>(colorB * m_Amount), 0);
            colorA = std::max(static_cast<int32_t>(colorA * m_Amount), 0);

            colorR = std::min(static_cast<int32_t>(Sml::colorGetR(originalColor)) + colorR, 255);
            colorG = std::min(static_cast<int32_t>(Sml::colorGetG(originalColor)) + colorG, 255);
            colorB = std::min(static_cast<int32_t>(Sml::colorGetB(originalColor)) + colorB, 255);
            colorA = std::min(static_cast<int32_t>(Sml::colorGetA(originalColor)) + colorA, 255);

            blurredImage[x + y * width] = Sml::rgbaColor(static_cast<uint8_t>(colorR),
                                                         static_cast<uint8_t>(colorG),
                                                         static_cast<uint8_t>(colorB),
                                                         static_cast<uint8_t>(colorA));
        }
    }

    renderer.getTarget()->updatePixels(blurredImage, nullptr);
}

int32_t SharpenFilter::getBlurRadius() const { return m_BlurKernel->getRadius(); }
void SharpenFilter::setBlurRadius(int32_t radius)
{
    if (radius != getBlurRadius())
    {
        delete m_BlurKernel;
        m_BlurKernel = Sml::createGaussianBlurKernel(radius);
    }
}

float SharpenFilter::getAmount() const { return m_Amount; }
void SharpenFilter::setAmount(float amount) { m_Amount = amount; }

Sml::Texture* SharpenFilter::getOriginalTexture() { return m_OriginalTexture; }

bool SharpenFilter::isCanceled() const { return m_Canceled; }
void SharpenFilter::setCanceled(bool canceled) { m_Canceled = canceled; }