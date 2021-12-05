/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file resource_manager.h
 * @date 2021-11-15
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include <unordered_map>
#include <string>
#include "sgl/scene/media/image.h"

#define RESOURCE_IMAGE(filename) ResourceManager::getInstance().getImage(filename)

class ResourceManager
{
public:
    static void init(const char* baseDir);
    static bool isInitialized();
    static ResourceManager& getInstance();

public:
    ~ResourceManager();

    const Sgl::Image* getImage(const std::string& filename);

private:
    static ResourceManager* s_Instance;

    std::unordered_map<std::string, Sgl::Image*> m_Images;
    const char*                                  m_BaseDir;

private:
    ResourceManager(const char* baseDir);

    std::string getFullName(const std::string& filename);
    void loadImage(const std::string& fullFilename);
};