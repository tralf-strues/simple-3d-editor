/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file filter.h
 * @date 2021-12-15
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "sml/sml_math.h"
#include "sml/sml_graphics_wrapper.h"
#include "sgl/containers.h"

namespace Paint
{
    class Filter
    {
        public:
        virtual ~Filter() = default;

        virtual const char* getName() const = 0;

        virtual Sgl::Container* getPreferencesPanel() { return nullptr; }

        virtual void init(Sml::Texture* texture) {}
        virtual void apply() = 0;
    };
}