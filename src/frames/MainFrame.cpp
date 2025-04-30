#include "MainFrame.h"
#include <wx/config.h>

MainFrame::MainFrame(const std::string& name)
	: wxFrame(NULL, wxID_ANY, name, wxDefaultPosition, { 900,700 }, wxCAPTION | wxCLOSE_BOX | wxMINIMIZE_BOX| wxMAXIMIZE_BOX |wxRESIZE_BORDER | wxNO_BORDER)
{
	//wxIcon icon;
	//icon.LoadFile("path/to/icon.ico", wxBITMAP_TYPE_ICO);  // Load icon from file
	//this->SetIcon(icon);

	CreateStatusBar();
	//SetStatusText("Welcome to wxWidgets!");
	CreateMenubar();

	notebook = new NotebookPanel(this);
}

void MainFrame::CreateMenubar()
{
	wxMenuBar* menubar = new wxMenuBar();

	// File
	wxMenu* file = new wxMenu();
	file->Append(ID_NewTab, "New Tab\tCtrl+N");
	file->Append(ID_OpenFile, "Open\tCtrl+O");
	//
	wxMenu* recent = new wxMenu();
	recent->Append(ID_ClearHistory, "Clear History");
	recent->AppendSeparator();
	file->AppendSubMenu(recent, "Recent");
	//
	file->AppendSeparator();
	file->Append(ID_Save, "Save\tCtrl+S");
	file->Append(ID_SaveAs, "SaveAs\tCtrl+Shift+S");
	file->Append(ID_SaveAll, "Save All\tCtrl+Alt+S");
	file->AppendSeparator();
	file->Append(ID_CloseTab, "Close Tab\tCtrl+W");
	file->Append(ID_Exit, "Exit");

	menubar->Append(file, "File");

	// Edit
	wxMenu* edit = new wxMenu();
	edit->Append(ID_Undo, "Undo\tCtrl+Z");
	edit->Append(ID_Redo, "Redo\tCtrl+Y");
	edit->AppendSeparator();
	edit->Append(ID_Cut, "Cut\tCtrl+X");
	edit->Append(ID_Copy, "Copy\tCtrl+C");
	edit->Append(ID_Paste, "Paste\tCtrl+V");
	edit->Append(ID_Delete, "Delete\tDel");
	edit->AppendSeparator();
	edit->Append(ID_Find, "Find\tCtrl+F");
	edit->Append(ID_Replace, "Replace\tCtrl+H");
	edit->AppendSeparator();
	edit->Append(ID_Font, "Font");

	menubar->Append(edit, "Edit");

	// Edit
	wxMenu* view = new wxMenu();
	wxMenu* zoomSubMenu = new wxMenu();
	zoomSubMenu->Append(ID_ZoomIn, "Zoom In\tCtrl+plus");
	zoomSubMenu->Append(ID_ZoomOut, "Zoom Out\tCtrl+minus");
	zoomSubMenu->Append(ID_ZoomReset, "Default Zoom (100%)\tCtrl+0");
	view->AppendSubMenu(zoomSubMenu, "Zoom");
	view->AppendCheckItem(ID_StatusBar, "Status Bar");
	view->AppendCheckItem(ID_WordWrap, "Word Warp");

	menubar->Append(view, "View");


	SetMenuBar(menubar);

	fileHistory.UseMenu(recent);
	fileHistory.Load(*wxConfig::Get());

	BindMenubarActions();
}

void MainFrame::BindMenubarActions()
{
	// New Tab
	Bind(wxEVT_MENU, &MainFrame::OnNewTab, this, ID_NewTab);
	// Exit
	Bind(wxEVT_MENU, [&](wxCommandEvent& e) 
		{ Close(); } 
		, ID_Exit);
	// Open
	Bind(wxEVT_MENU, &MainFrame::OnOpen, this, ID_OpenFile);
	// Recents
	Bind(wxEVT_MENU, [&](wxCommandEvent& e)
		{
			const auto historyId = e.GetId() - fileHistory.GetBaseId();
			const wxString filepath = fileHistory.GetHistoryFile(historyId);
			wxMessageBox("Opening: " + filepath);
		},
		fileHistory.GetBaseId(),
		fileHistory.GetBaseId() + fileHistory.GetMaxFiles()
	);
	// Clear Recent
	Bind(wxEVT_MENU, [&](wxCommandEvent& e) {
		while (fileHistory.GetCount() > 0) 
			fileHistory.RemoveFileFromHistory(0);
		fileHistory.Save(*wxConfig::Get());
		}, ID_ClearHistory);
	// Save 
	Bind(wxEVT_MENU, &MainFrame::OnSave, this, ID_Save);
	Bind(wxEVT_MENU, &MainFrame::OnSaveAs, this, ID_SaveAs);

}

void MainFrame::OnNewTab(wxCommandEvent& e)
{
	notebook->CreateNewTab("Untitled.txt");
}

void MainFrame::OnOpen(wxCommandEvent&)
{
	wxFileDialog openFileDialog(this, "Open File", "", "", "Text Document (*.txt)|*.txt|All Files (*.*)|*.*", wxFD_FILE_MUST_EXIST | wxFD_OPEN);

	if (openFileDialog.ShowModal() == wxID_OK) {
		wxString filepath = openFileDialog.GetPath();
		wxMessageBox("Opening: " + filepath);
		fileHistory.AddFileToHistory(filepath);
		fileHistory.Save(*wxConfig::Get());
	}
}

void MainFrame::OnSave(wxCommandEvent&)
{
	notebook->SaveCurrentTab();
}

void MainFrame::OnSaveAs(wxCommandEvent&)
{
	notebook->SaveAsCurrentTab();
}
