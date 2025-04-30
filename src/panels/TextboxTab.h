#pragma once

#include <wx/wx.h>
#include <wx/stc/stc.h>
#include <wx/aui/aui.h>

class TextboxTab : public wxPanel
{
public:
	TextboxTab(wxAuiNotebook* parent, const wxString& filename);

	void OnSaveFile(); 
	void OnSaveAsFile();
private:
	wxStyledTextCtrl* editor;
	std::string filepath;
	std::string filename;
	bool isModified = false;

	void SaveFile(const std::string& filepath);
	void UpdateTabLabel();
};
