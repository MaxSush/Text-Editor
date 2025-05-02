#pragma once

#include <wx/wx.h>
#include <wx/aui/aui.h>
#include <wx/stc/stc.h>
#include <vector>
#include "TextboxTab.h"

class NotebookPanel : public wxPanel
{
public:
	NotebookPanel(wxWindow* parent);
	void CreateNewTab(const wxString& filename);

	void SaveAllTab();
	void CloseCurrentTab();

	TextboxTab* currentTab = nullptr;

private:
	wxAuiNotebook* notebook;
	std::vector<TextboxTab*> Openedtabs;

	void RemoveTab();
	TextboxTab* GetCurrentTab();
};