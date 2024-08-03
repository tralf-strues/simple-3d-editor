/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file basic_filters.h
 * @date 2021-12-15
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "sml/math/blur.h"
#include "filter.h"

namespace Paint
{
    class SharpenFilter : public Filter
    {
    public:
        SharpenFilter() = default;
        virtual ~SharpenFilter() override;

        virtual const char* getName() const override;

        virtual Sgl::Container* getPreferencesPanel() override;

        virtual void init(Sml::Texture* texture) override;
        virtual void apply() override;

        int32_t getBlurRadius() const;
        void setBlurRadius(int32_t radius);

        float getAmount() const;
        void setAmount(float amount);

        Sml::Texture* getOriginalTexture();

        bool isCanceled() const;
        void setCanceled(bool canceled);

    private:
        Sml::Texture* m_OriginalTexture = nullptr;
        Sml::Kernel*  m_BlurKernel      = Sml::createGaussianBlurKernel(5);
        float         m_Amount          = 1.5f;
        bool          m_Canceled        = false;
    };
};