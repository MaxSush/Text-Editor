#pragma once

#include <wx/wx.h>
#include <wx/stc/stc.h>
#include <wx/aui/aui.h>
#include <wx/config.h>
#include <wx/file.h>
#include "../controls/StatusBar.h"

class TextboxTab : public wxPanel
{
public:
	TextboxTab(wxAuiNotebook* parent, const wxString& filename);

	void OnSaveFile(); 
	void OnSaveAsFile();
	void SetFileContents(const wxString& filepath);

	void OnUndo();
	void OnRedo();
	void OnCut();
	void OnCopy();
	void OnPaste();
	void OnDelete();
	void OnChangeFont(const wxFont& font);
	void OnSetDefaultFont();
	void OnResetFont();
	void OnZoomIn();
	void OnZoomOut();
	void OnSetZoom();

	void OnFind();
	void OnReplace();

	void UpdateStatusbar();
	void OnWordWrap(bool warp);
	std::string GetFilepath() const;
private:
	StatusBar* statusBar = nullptr;
	wxStyledTextCtrl* editor;
	std::string m_filepath;
	std::string m_filename;
	bool isModified = false;
	bool suppressChangeEvent = false;

	void SaveFile();
	void UpdateTabLabel();
	void BindActions();
};
