#include "TextboxTab.h"

TextboxTab::TextboxTab(wxAuiNotebook* parent, const wxString& filename)
	:
	wxPanel(parent, wxID_ANY),
	filename(filename)
{
	editor = new wxStyledTextCtrl(this, wxID_ANY);
	editor->Bind(wxEVT_STC_CHANGE, [=](wxStyledTextEvent&) {
		isModified = true;
		UpdateTabLabel();
		});

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(editor, 1, wxEXPAND);
	this->SetSizer(sizer);

	parent->AddPage(this, this->filename, true);
}

void TextboxTab::OnSaveFile()
{
	if (filepath.empty())
		OnSaveAsFile();
	else
		SaveFile(filepath);
}

void TextboxTab::OnSaveAsFile()
{
	wxFileDialog saveDialog(this, "Save As", "", "",
		"Text files (*.txt)|*.txt|All files (*.*)|*.*",
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (saveDialog.ShowModal() == wxID_OK) {
		filepath = saveDialog.GetPath();
		filename = saveDialog.GetFilename();
		SaveFile(filepath);
	}
}

void TextboxTab::SaveFile(const std::string& filepath)
{
	isModified = false;
	UpdateTabLabel();
	wxMessageBox("File Saved At: " + filepath);
}

void TextboxTab::UpdateTabLabel()
{
	wxAuiNotebook* parentNotebook = wxDynamicCast(GetParent(), wxAuiNotebook);
	if (!parentNotebook) return;

	int pageIndex = parentNotebook->FindPage(this);
	if (pageIndex == wxNOT_FOUND) return;

	if (isModified) {
		parentNotebook->SetPageText(pageIndex, "*" + filename);
	}
	else {
		parentNotebook->SetPageText(pageIndex, filename);
	}
}
