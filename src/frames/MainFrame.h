#pragma once

#include <wx/wx.h>

#include "../panels/NotebookPanel.h"

class MainFrame : public wxFrame
{
public:
	MainFrame(const std::string& name);

private:
	NotebookPanel* notebook;

	void CreateMenubar();
	void BindMenubarActions();

	void OnNewTab(wxCommandEvent&);
};
