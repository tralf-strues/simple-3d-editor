/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file filter.h
 * @date 2021-12-15
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

class Filter
{
public:
    virtual ~Filter() = default;

    virtual void apply(ITexture* canvas) = 0;
    virtual const char* getName() const = 0;
};