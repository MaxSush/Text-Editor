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

	void SaveCurrentTab();
	void SaveAsCurrentTab();
	void SaveAllCurrentTab();

private:
	wxAuiNotebook* notebook;
	std::vector<TextboxTab*> Openedtabs;

	TextboxTab* GetCurrentTab();
	void OnNotebookTabClose(wxCommandEvent& e);
};