/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file paint_editor.h
 * @date 2021-12-05
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include <list>
#include "document.h"
#include "tool.h"

namespace Paint
{
    class Editor
    {
    public:
        static void init();
        static bool isInitialized();
        static Editor& getInstance();

    public:
        ~Editor();

        Tool* getActiveTool();
        void setActiveTool(Tool* tool); ///< The tool doesn't have to be in the tools list!

        const std::list<Tool*>& getTools() const;
        void addTool(Tool* tool);

        Document* getActiveDocument();
        void setActiveDocument(Document* document); ///< The document must be in the documents list!

        const std::list<Document*>& getDocuments() const;
        void addDocument(Document* document);

    private:
        static Editor*       s_Instance;

        Tool*                m_ActiveTool     = nullptr;
        std::list<Tool*>     m_Tools;

        Document*            m_ActiveDocument = nullptr;
        std::list<Document*> m_Documents;

    private:
        Editor() = default;
    };
};