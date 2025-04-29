#include "MainFrame.h"

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
	file->Append(wxID_NEW, "&New Tab\t Ctrl+N");
	file->Append(wxID_OPEN, "&Open\t Ctrl+O");
	//
	file->Append(wxID_ANY, "&Recent");
	//
	file->AppendSeparator();
	file->Append(wxID_SAVE, "&Save\t Ctrl+S");
	file->Append(wxID_SAVEAS, "&SaveAs\t Ctrl+Shift+S");
	file->Append(wxID_ANY, "&Save All\t Ctrl+Alt+S");
	file->AppendSeparator();
	file->Append(wxID_CLOSE, "Close Tab\t Ctrl+W");
	file->Append(wxID_EXIT, "Exit");

	menubar->Append(file, "File");

	// Edit
	wxMenu* edit = new wxMenu();
	edit->Append(wxID_UNDO, "&Undo\t Ctrl+Z");
	edit->Append(wxID_REDO, "&Redo\t Ctrl+Y");
	edit->AppendSeparator();
	edit->Append(wxID_CUT, "&Cut\t Ctrl+X");
	edit->Append(wxID_COPY, "&Copy\t Ctrl+C");
	edit->Append(wxID_PASTE, "&Paste\t Ctrl+V");
	edit->Append(wxID_DELETE, "&Delete\t Del");
	edit->AppendSeparator();
	edit->Append(wxID_FIND, "&Find\t Ctrl+F");
	edit->Append(wxID_REPLACE, "&Replace\t Ctrl+H");
	edit->AppendSeparator();
	edit->Append(wxID_ANY, "&Font");

	menubar->Append(edit, "Edit");

	// Edit
	wxMenu* view = new wxMenu();
	wxMenu* zoomSubMenu = new wxMenu();
	zoomSubMenu->Append(wxID_ZOOM_IN, "&Zoom In\t Ctrl+plus");
	zoomSubMenu->Append(wxID_ZOOM_OUT, "&Zoom Out\t Ctrl+minus");
	zoomSubMenu->Append(wxID_ZOOM_100, "&Default Zoom (100%)\t Ctrl+0");
	view->AppendSubMenu(zoomSubMenu, "&Zoom");
	view->AppendCheckItem(wxID_ANY, "&Status Bar", "Display Status Bar");
	view->AppendCheckItem(wxID_ANY, "&Word Warp");

	menubar->Append(view, "View");


	SetMenuBar(menubar);

	BindMenubarActions();
}

void MainFrame::BindMenubarActions()
{
	Bind(wxEVT_MENU, &MainFrame::OnNewTab, this, wxID_NEW);
}

void MainFrame::OnNewTab(wxCommandEvent& e)
{
	notebook->CreateNewTab("Untitled.txt");
}
