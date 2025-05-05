#include "MainFrame.h"
#include <wx/config.h>
#include <wx/fontdlg.h>
#include <wx/filename.h>


MainFrame::MainFrame(const std::string& name)
	: wxFrame(NULL, wxID_ANY, name, wxDefaultPosition, { 900,700 }, wxCAPTION | wxCLOSE_BOX | wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxRESIZE_BORDER | wxNO_BORDER)
{
	//wxIcon icon;
	//icon.LoadFile("path/to/icon.ico", wxBITMAP_TYPE_ICO);  // Load icon from file
	//this->SetIcon(icon);

	fileHistory = new wxFileHistory(10);
	
	menubar = new MenuBar(fileHistory);
	SetMenuBar(menubar);
	BindMenubarActions();

	statusBar = new StatusBar(this);
	SetStatusBar(statusBar);
	wxConfigBase* config = wxConfig::Get();
	config->Read("StatusBar", &isStatusbar, true);
	if (!isStatusbar){
		statusBar->Hide();
		SendSizeEvent();
	}
	menubar->Check(ID_StatusBar, isStatusbar);

	notebookPanel = new NotebookPanel(this);
}

// Destructor
MainFrame::~MainFrame()
{
	delete fileHistory;
}

void MainFrame::BindMenubarActions()
{
	// File

	// New Tab
	Bind(wxEVT_MENU, [&](wxCommandEvent& e) {
		menubar->Check(ID_WordWrap, true);
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
		TextboxTab* tab = notebookPanel->currentTab;
		tab->OnSaveFile();
		}, ID_Save);
	// Save As
	Bind(wxEVT_MENU, [&](wxCommandEvent& e) {
		TextboxTab* tab = notebookPanel->currentTab;
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
		TextboxTab* tab = notebookPanel->currentTab;
		tab->OnUndo();
		}, ID_Undo);

	// Redo
	Bind(wxEVT_MENU, [&](wxCommandEvent& e) {
		TextboxTab* tab = notebookPanel->currentTab;
		tab->OnRedo();
		}, ID_Redo);

	// Cut
	Bind(wxEVT_MENU, [&](wxCommandEvent& e) {
		TextboxTab* tab = notebookPanel->currentTab;
		tab->OnCut();
		}, ID_Cut);

	// Copy
	Bind(wxEVT_MENU, [&](wxCommandEvent& e) {
		TextboxTab* tab = notebookPanel->currentTab;
		tab->OnCopy();
		}, ID_Copy);

	// Paste
	Bind(wxEVT_MENU, [&](wxCommandEvent& e) {
		TextboxTab* tab = notebookPanel->currentTab;
		tab->OnPaste();
		}, ID_Paste);

	// Delete
	Bind(wxEVT_MENU, [&](wxCommandEvent& e) {
		TextboxTab* tab = notebookPanel->currentTab;
		tab->OnDelete();
		}, ID_Delete);

	// Delete
	Bind(wxEVT_MENU, &MainFrame::OnChangeFont, this, ID_Font);
	Bind(wxEVT_MENU, &MainFrame::OnSetDefaultFont, this, ID_SetFont);
	Bind(wxEVT_MENU, [&](wxCommandEvent& e) {
		notebookPanel->currentTab->OnResetFont();
		}, ID_ResetFont);

	// Zoom In
	Bind(wxEVT_MENU, [&](wxCommandEvent& e) {
		notebookPanel->currentTab->OnZoomIn();
		}, ID_ZoomIn);

	// Zoom Ot
	Bind(wxEVT_MENU, [&](wxCommandEvent& e) {
		notebookPanel->currentTab->OnZoomOut();
		}, ID_ZoomOut);

	// Set Zoom
	Bind(wxEVT_MENU, [&](wxCommandEvent& e) {
		notebookPanel->currentTab->OnSetZoom();
		}, ID_ZoomReset);

	// StatusBar
	Bind(wxEVT_MENU, &MainFrame::OnStatusbar, this,  ID_StatusBar);

	// Word Wrap
	Bind(wxEVT_MENU, [&](wxCommandEvent& e) {
		notebookPanel->currentTab->OnWordWrap(e.IsChecked());
		}, ID_WordWrap);
}


void MainFrame::OnOpen(wxCommandEvent& e)
{
	wxFileDialog openFileDialog(this, "Open File", "", "", "Text Document (*.txt)|*.txt|All Files (*.*)|*.*", wxFD_FILE_MUST_EXIST | wxFD_OPEN);

	if (openFileDialog.ShowModal() == wxID_OK) {
		wxString filepath = openFileDialog.GetPath();
		wxString filename = openFileDialog.GetFilename();
		notebookPanel->OpenFile(filename, filepath);
		fileHistory->AddFileToHistory(filepath);
		fileHistory->Save(*wxConfig::Get());
	}
}

void MainFrame::OnRecents(wxCommandEvent& e)
{
	const auto historyId = e.GetId() - fileHistory->GetBaseId();
	const wxString filepath = fileHistory->GetHistoryFile(historyId);
	wxFileName filename(filepath);
	if (!filename.FileExists()) {
		wxMessageBox("The file \"" + filepath + "\" does not exist.", "File Not Found", wxICON_ERROR);
		return;
	}
	notebookPanel->OpenFile(filename.GetFullName(), filepath);
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
		TextboxTab* tab = notebookPanel->currentTab;
		if (tab)
			tab->OnChangeFont(font);
	}
}

void MainFrame::OnSetDefaultFont(wxCommandEvent& e)
{
	TextboxTab* tab = notebookPanel->currentTab;
	tab->OnSetDefaultFont();
	wxMessageBox("The current font has been set as the default. \n Please restart the application for the changes to take effect throughout.");
}

void MainFrame::OnStatusbar(wxCommandEvent& e)
{
	bool show = e.IsChecked();
	statusBar->Show(show);
	SendSizeEvent();
	isStatusbar = show;

	wxConfigBase* config = wxConfig::Get();
	config->Write("StatusBar", isStatusbar);
}
