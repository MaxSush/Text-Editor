#include "TextboxTab.h"

TextboxTab::TextboxTab(wxAuiNotebook* parent, const wxString& filename)
	:
	wxPanel(parent, wxID_ANY),
	filename(filename)
{
	editor = new wxStyledTextCtrl(this, wxID_ANY);
	editor->SetWrapMode(wxSTC_WRAP_WORD);

	editor->Bind(wxEVT_STC_CHANGE, [=](wxStyledTextEvent&) {
		isModified = true;
		UpdateTabLabel();
		});

	{
		wxString faceName;
		int pointSize;
		bool bold, italic;
		wxConfigBase* config = wxConfig::Get();

		config->Read("Font/FaceName", &faceName, "Consolas");
		config->Read("Font/PointSize", &pointSize, 14);
		config->Read("Font/Bold", &bold, false);
		config->Read("Font/Italic", &italic, false);

		wxFont font(pointSize, wxFONTFAMILY_MODERN, (italic) ? wxFONTSTYLE_ITALIC : wxFONTSTYLE_NORMAL, (bold) ? wxFONTWEIGHT_BOLD : wxFONTWEIGHT_NORMAL);
		font.SetFaceName(faceName);
		editor->StyleSetFont(wxSTC_STYLE_DEFAULT, font);
		editor->StyleClearAll();
	}

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
		if (isModified)
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

void TextboxTab::OnUndo()
{
	if (editor->CanUndo())
		editor->Undo();
}

void TextboxTab::OnRedo()
{
	if (editor->CanRedo())
		editor->Redo();
}

void TextboxTab::OnCut()
{
	if (editor->CanCut())
		editor->Cut();
}

void TextboxTab::OnCopy()
{
	if (editor->CanCopy())
		editor->Copy();
}

void TextboxTab::OnPaste()
{
	if (editor->CanPaste())
		editor->Paste();
}

void TextboxTab::OnDelete()
{
	editor->Clear();
}

void TextboxTab::OnChangeFont(const wxFont& font)
{
	editor->StyleSetFont(wxSTC_STYLE_DEFAULT, font);
	editor->StyleClearAll();
}

void TextboxTab::OnSetDefaultFont()
{
	wxConfigBase* config = wxConfig::Get();
	wxFont font = editor->StyleGetFont(wxSTC_STYLE_DEFAULT);
	if (font.IsOk()) {
		config->Write("Font/FaceName", font.GetFaceName());
		config->Write("Font/PointSize", font.GetPointSize());
		config->Write("Font/Bold", font.GetWeight() == wxFONTWEIGHT_BOLD);
		config->Write("Font/Italic", font.GetStyle() == wxFONTSTYLE_ITALIC);
	}
}

void TextboxTab::OnResetFont()
{
	wxFont font(14, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	font.SetFaceName("Consolas");
	editor->StyleSetFont(wxSTC_STYLE_DEFAULT, font);
	editor->StyleClearAll();
	OnSetDefaultFont();
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
