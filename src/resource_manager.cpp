/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file resource_manager.cpp
 * @date 2021-11-15
 * 
 * @copyright Copyright (c) 2021
 */

#include "resource_manager.h"

// Sgl::Image* Resources::ICON_CLOSE_IDLE    = nullptr;
// Sgl::Image* Resources::ICON_CLOSE_HOVERED = nullptr;

// void Resources::load()
// {
//     ICON_CLOSE_IDLE    = new Sgl::Image("res/icons/close_idle_32.png",    Sgl::ImageFormat::PNG);
//     ICON_CLOSE_HOVERED = new Sgl::Image("res/icons/close_hovered_32.png", Sgl::ImageFormat::PNG);
// }

// void Resources::close()
// {
//     delete ICON_CLOSE_IDLE;
//     delete ICON_CLOSE_HOVERED;
// }

ResourceManager* ResourceManager::s_Instance = nullptr;

void ResourceManager::init(const char* baseDir)
{
    assert(baseDir);
    s_Instance = new ResourceManager(baseDir);
}

bool ResourceManager::isInitialized()
{
    return s_Instance != nullptr;
}

ResourceManager& ResourceManager::getInstance()
{
    return *s_Instance;
}

ResourceManager::ResourceManager(const char* baseDir) : m_BaseDir(baseDir) { assert(m_BaseDir); }

ResourceManager::~ResourceManager()
{
    for (auto image : m_Images)
    {
        delete image.second;
    }
}

const Sgl::Image* ResourceManager::getImage(const std::string& filename)
{
    std::string fullname = getFullName(filename);

    auto foundImage = m_Images.find(fullname);

    if (foundImage == m_Images.end())
    {
        loadImage(fullname);
        foundImage = m_Images.find(fullname);
    }

    return (*foundImage).second;
}

std::string ResourceManager::getFullName(const std::string& filename)
{
    std::string fullFilename = m_BaseDir;
    fullFilename += filename;

    return fullFilename;
}

void ResourceManager::loadImage(const std::string& fullFilename)
{
    m_Images[fullFilename] = new Sgl::Image(fullFilename.c_str(), Sgl::ImageFormat::PNG);
}