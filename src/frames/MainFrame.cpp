#include "MainFrame.h"
#include <wx/config.h>
#include <wx/fontdlg.h>

MainFrame::MainFrame(const std::string& name)
	: wxFrame(NULL, wxID_ANY, name, wxDefaultPosition, { 900,700 }, wxCAPTION | wxCLOSE_BOX | wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxRESIZE_BORDER | wxNO_BORDER)
{
	//wxIcon icon;
	//icon.LoadFile("path/to/icon.ico", wxBITMAP_TYPE_ICO);  // Load icon from file
	//this->SetIcon(icon);

	CreateStatusBar();
	//SetStatusText("Welcome to wxWidgets!");

	notebookPanel = new NotebookPanel(this);
	fileHistory = new wxFileHistory(10);

	CreateMenubar();
}

MainFrame::~MainFrame()
{
	delete fileHistory;
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
	edit->Append(ID_SetFont, "Set Current Font Default");
	edit->Append(ID_ResetFont, "Reset Font");

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

	fileHistory->UseMenu(recent);
	fileHistory->Load(*wxConfig::Get());

	BindMenubarActions();
}

void MainFrame::BindMenubarActions()
{
	// File

	// New Tab
	Bind(wxEVT_MENU, [&](wxCommandEvent& e) {
		notebookPanel->CreateNewTab("Untitled.txt");
		}, ID_NewTab);
	// Exit
	Bind(wxEVT_MENU, [&](wxCommandEvent& e) {
		Close();
		}, ID_Exit);
	// Open
	Bind(wxEVT_MENU, &MainFrame::OnOpen, this, ID_OpenFile);
	// Recents
	Bind(wxEVT_MENU, &MainFrame::OnRecents, this, fileHistory->GetBaseId(), fileHistory->GetBaseId() + fileHistory->GetMaxFiles());
	// Clear Recent
	Bind(wxEVT_MENU, &MainFrame::OnClearRecents, this, ID_ClearHistory);
	// Save 
	Bind(wxEVT_MENU, [&](wxCommandEvent& e) {
		TextboxTab* tab = notebookPanel->GetCurrentTab();
		tab->OnSaveFile();
		}, ID_Save);
	// Save As
	Bind(wxEVT_MENU, [&](wxCommandEvent& e) {
		TextboxTab* tab = notebookPanel->GetCurrentTab();
		tab->OnSaveAsFile();
		}, ID_SaveAs);
	// Save All
	Bind(wxEVT_MENU, [&](wxCommandEvent& e) {
		notebookPanel->SaveAllTab();
		}, ID_SaveAll);
	// Close Tab
	Bind(wxEVT_MENU, [&](wxCommandEvent& e) {
		notebookPanel->CloseCurrentTab();
		}, ID_CloseTab);

	// Edit

	// Undo
	Bind(wxEVT_MENU, [&](wxCommandEvent& e) {
		TextboxTab* tab = notebookPanel->GetCurrentTab();
		tab->OnUndo();
		}, ID_Undo);

	// Redo
	Bind(wxEVT_MENU, [&](wxCommandEvent& e) {
		TextboxTab* tab = notebookPanel->GetCurrentTab();
		tab->OnRedo();
		}, ID_Redo);

	// Cut
	Bind(wxEVT_MENU, [&](wxCommandEvent& e) {
		TextboxTab* tab = notebookPanel->GetCurrentTab();
		tab->OnCut();
		}, ID_Cut);

	// Copy
	Bind(wxEVT_MENU, [&](wxCommandEvent& e) {
		TextboxTab* tab = notebookPanel->GetCurrentTab();
		tab->OnCopy();
		}, ID_Copy);

	// Paste
	Bind(wxEVT_MENU, [&](wxCommandEvent& e) {
		TextboxTab* tab = notebookPanel->GetCurrentTab();
		tab->OnPaste();
		}, ID_Paste);

	// Delete
	Bind(wxEVT_MENU, [&](wxCommandEvent& e) {
		TextboxTab* tab = notebookPanel->GetCurrentTab();
		tab->OnDelete();
		}, ID_Delete);

	// Delete
	Bind(wxEVT_MENU, &MainFrame::OnChangeFont, this, ID_Font);
	Bind(wxEVT_MENU, &MainFrame::OnSetDefaultFont, this, ID_SetFont);
	Bind(wxEVT_MENU, [&](wxCommandEvent& e) {
		TextboxTab* tab = notebookPanel->GetCurrentTab();
		tab->OnResetFont();
		}, ID_ResetFont);
}


void MainFrame::OnOpen(wxCommandEvent& e)
{
	wxFileDialog openFileDialog(this, "Open File", "", "", "Text Document (*.txt)|*.txt|All Files (*.*)|*.*", wxFD_FILE_MUST_EXIST | wxFD_OPEN);

	if (openFileDialog.ShowModal() == wxID_OK) {
		wxString filepath = openFileDialog.GetPath();
		wxMessageBox("Opening: " + filepath);
		fileHistory->AddFileToHistory(filepath);
		fileHistory->Save(*wxConfig::Get());
	}
}

void MainFrame::OnRecents(wxCommandEvent& e)
{
	const auto historyId = e.GetId() - fileHistory->GetBaseId();
	const wxString filepath = fileHistory->GetHistoryFile(historyId);
	wxMessageBox("Opening: " + filepath);
}

void MainFrame::OnClearRecents(wxCommandEvent& e)
{
	while (fileHistory->GetCount() > 0)
		fileHistory->RemoveFileFromHistory(0);
	fileHistory->Save(*wxConfig::Get());
}

void MainFrame::OnChangeFont(wxCommandEvent& e)
{
	wxFontDialog fontDialog(this);
	if (fontDialog.ShowModal() == wxID_OK) {
		wxFont font = fontDialog.GetFontData().GetChosenFont();
		TextboxTab* tab = notebookPanel->GetCurrentTab();
		if (tab)
			tab->OnChangeFont(font);
	}
}

void MainFrame::OnSetDefaultFont(wxCommandEvent& e)
{
	TextboxTab* tab = notebookPanel->GetCurrentTab();
	tab->OnSetDefaultFont();
	wxMessageBox("The current font has been set as the default. \n Please restart the application for the changes to take effect throughout.");
}
