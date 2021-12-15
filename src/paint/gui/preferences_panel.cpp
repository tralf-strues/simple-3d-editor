/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file preferences_panel.cpp
 * @date 2021-12-15
 * 
 * @copyright Copyright (c) 2021
 */

#include "paint/gui/preferences_panel.h"

using namespace Paint;

const Sgl::ColorFill  PreferencesPanel::DEFAULT_FILL       = Sgl::ColorFill(0xEC'E8'ED'FF);
const Sgl::Background PreferencesPanel::DEFAULT_BACKGROUND = Sgl::Background(&DEFAULT_FILL);

PreferencesPanel::PreferencesPanel() : m_View(new Sgl::Container()) { m_View->setBackground(&DEFAULT_BACKGROUND); }

Sgl::Container* PreferencesPanel::getView() { return m_View; }

void PreferencesPanel::update()
{
    Tool* activeTool = Editor::getInstance().getActiveTool();
    
    if (activeTool == m_Tool)
    {
        return;
    }

    for (auto child : m_View->getChildren())
    {
        delete child;
    }

    m_View->removeChildren();

    Sgl::Container* panel = activeTool->getPreferencesPanel();

    if (panel != nullptr)
    {
        m_View->addChild(panel);
    }

    m_Tool = activeTool;
}