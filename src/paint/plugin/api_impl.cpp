/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file api_impl.cpp
 * @date 2021-12-14
 * 
 * @copyright Copyright (c) 2021
 */

#include "paint/plugin/api_impl.h"

using namespace plugin;

APIImpl::APIImpl() : m_TextureFactory(new TextureFactoryImpl()), m_WidgetFactory(new WidgetFactoryImpl()) {}

APIImpl::~APIImpl()
{
    delete m_TextureFactory;
    delete m_WidgetFactory;
}

IWidgetFactory* APIImpl::GetWidgetFactory() { return m_WidgetFactory; }
ITextureFactory* APIImpl::GetTextureFactory() { return m_TextureFactory; }