/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file api_impl.h
 * @date 2021-12-14
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "texture_impl.h"
#include "widget_impl.h"

namespace plugin
{
    class APIImpl : public IAPI
    {
    public:
        APIImpl();
        virtual ~APIImpl() override;

        virtual IWidgetFactory*  GetWidgetFactory () override;
        virtual ITextureFactory* GetTextureFactory() override;

    private:
        TextureFactoryImpl* m_TextureFactory = nullptr;
        WidgetFactoryImpl*  m_WidgetFactory  = nullptr;
    };
};