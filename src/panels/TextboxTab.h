#pragma once

#include <wx/wx.h>
#include <wx/stc/stc.h>
#include <wx/aui/aui.h>
#include <wx/config.h>
#include "../controls/StatusBar.h"

class TextboxTab : public wxPanel
{
public:
	TextboxTab(wxAuiNotebook* parent, const wxString& filename);

	void OnSaveFile(); 
	void OnSaveAsFile();

	void OnUndo();
	void OnRedo();
	void OnCut();
	void OnCopy();
	void OnPaste();
	void OnDelete();
	void OnChangeFont(const wxFont& font);
	void OnSetDefaultFont();
	void OnResetFont();

	void OnFind();
	void OnReplace();

	void OnZoomIn();
	void OnZoomOut();
	void OnSetZoom();

	void UpdateStatusbar();
	void OnWordWrap(bool warp);

private:
	StatusBar* statusBar = nullptr;
	wxStyledTextCtrl* editor;
	std::string filepath;
	std::string filename;
	bool isModified = false;

	void SaveFile(const std::string& filepath);
	void UpdateTabLabel();
};
