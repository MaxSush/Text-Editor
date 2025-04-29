#pragma once

#include <wx/wx.h>
#include <wx/aui/aui.h>
#include <wx/stc/stc.h>

class NotebookPanel : public wxPanel
{
public:
	NotebookPanel(wxWindow* parent);

	void CreateNewTab(const std::string& filename);
	int GetPageCount() const;

private:
	wxAuiNotebook* notebook;
};