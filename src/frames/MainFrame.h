#pragma once

#include <wx/wx.h>
#include <wx/filehistory.h>

#include "../panels/NotebookPanel.h"
#include "../controls/StatusBar.h"
#include "../controls/MenuBar.h"


class MainFrame : public wxFrame
{
public:
	MainFrame(const std::string& name);
    ~MainFrame();

private:
    StatusBar* statusBar = nullptr;
    bool isStatusbar = true;
	wxFileHistory* fileHistory = nullptr;
    MenuBar* menubar;
    NotebookPanel* notebookPanel;

	void BindMenubarActions();

	void OnOpen(wxCommandEvent&);
	void OnRecents(wxCommandEvent&);
	void OnClearRecents(wxCommandEvent&);
    void OnChangeFont(wxCommandEvent&);
    void OnSetDefaultFont(wxCommandEvent&);
    void OnStatusbar(wxCommandEvent&);

};
