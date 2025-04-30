#pragma once

#include <wx/wx.h>
#include <wx/filehistory.h>

#include "../panels/NotebookPanel.h"


enum {
    ID_NewTab = wxID_HIGHEST + 1,
    ID_OpenFile,
    ID_Save,
    ID_SaveAs,
    ID_SaveAll,
    ID_CloseTab,
    ID_Exit,
    ID_Undo,
    ID_Redo,
    ID_Cut,
    ID_Copy,
    ID_Paste,
    ID_Delete,
    ID_Find,
    ID_Replace,
    ID_Font,
    ID_StatusBar,
    ID_WordWrap,
    ID_ZoomIn,
    ID_ZoomOut,
    ID_ZoomReset,
    ID_ClearHistory
};


class MainFrame : public wxFrame
{
public:
	MainFrame(const std::string& name);

private:
	NotebookPanel* notebook;
	wxFileHistory fileHistory;


	void CreateMenubar();
	void BindMenubarActions();

	void OnNewTab(wxCommandEvent&);
	void OnOpen(wxCommandEvent&);
	void OnSave(wxCommandEvent&);
	void OnSaveAs(wxCommandEvent&);
};
