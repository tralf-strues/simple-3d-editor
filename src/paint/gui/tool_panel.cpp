/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file tool_panel.cpp
 * @date 2021-12-06
 * 
 * @copyright Copyright (c) 2021
 */

#include "resource_manager.h"
#include "paint/gui/tool_panel.h"
#include "paint/paint_editor.h"
#include "sgl/scene/controls/button.h"
#include "sgl/scene/controls/label.h"

using namespace Paint;

const Sgl::Insets     ToolPanel::DEFAULT_PADDING         = {5, 3};
const int32_t         ToolPanel::DEFAULT_SPACING         = 3;
const Sgl::Border     ToolPanel::DEFAULT_BORDER          = {1, 0xC9'C9'C9'66};
const Sgl::ColorFill  ToolPanel::DEFAULT_BACKGROUND_FILL = {0xEE'EE'EE'FF};
const Sgl::Background ToolPanel::DEFAULT_BACKGROUND      = {&DEFAULT_BACKGROUND_FILL};
const int32_t         ToolPanel::DEFAULT_HGAP            = 2;
const int32_t         ToolPanel::DEFAULT_VGAP            = 2;

ToolPanel::ToolPanel() : m_View(new Sgl::VBox()), m_Tools(new Sgl::TilePane()), m_ColorPicker(new ColorPickerWidget())
{
    getView()->setPadding(DEFAULT_PADDING);
    getView()->setBorder(&DEFAULT_BORDER);
    getView()->setBackground(&DEFAULT_BACKGROUND);
    getView()->setSpacing(DEFAULT_SPACING);
    m_Tools->setBackground(&DEFAULT_BACKGROUND);
    m_Tools->setHgap(DEFAULT_HGAP);
    m_Tools->setVgap(DEFAULT_VGAP);
    // m_Tools->setPrefTileWidth(32);
    // m_Tools->setPrefTileHeight(32);

    m_View->addChild(new Sgl::Text("Tools"));
    m_View->addChild(m_Tools);
    m_View->addChild(m_ColorPicker);
}

ToolPanel::~ToolPanel()
{
    delete m_View;
    delete m_Tools;
}

Sgl::VBox* ToolPanel::getView()
{
    return m_View;
}

void ToolPanel::loadTools()
{
    for (auto tool : Editor::getInstance().getTools())
    {
        LOG_APP_INFO("Loading tool to ToolPanel {name = '%s', icon = '%s'}", tool->getName(), tool->getIconFilename());
        m_Tools->addChild(new Sgl::Button(RESOURCE_IMAGE(tool->getIconFilename())));
    }
}