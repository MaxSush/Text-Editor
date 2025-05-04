#include "MenuBar.h"
#include <wx/config.h>


MenuBar::MenuBar(wxFileHistory* fileHistory)
	:
	wxMenuBar()
{
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

	Append(file, "File");

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

	Append(edit, "Edit");

	// View
	wxMenu* view = new wxMenu();
	wxMenu* zoomSubMenu = new wxMenu();
	zoomSubMenu->Append(ID_ZoomIn, "Zoom In\tCtrl+=");
	zoomSubMenu->Append(ID_ZoomOut, "Zoom Out\tCtrl+-");
	zoomSubMenu->Append(ID_ZoomReset, "Default Zoom (100%)\tCtrl+0");
	view->AppendSubMenu(zoomSubMenu, "Zoom");
	view->AppendCheckItem(ID_StatusBar, "Status Bar");
	view->AppendCheckItem(ID_WordWrap, "Word Warp");

	Append(view, "View");
	Check(ID_WordWrap, true);

	fileHistory->UseMenu(recent);
	fileHistory->Load(*wxConfig::Get());
}
