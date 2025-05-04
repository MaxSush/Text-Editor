#include "TextboxTab.h"

TextboxTab::TextboxTab(wxAuiNotebook* parent, const wxString& filename)
	:
	wxPanel(parent, wxID_ANY),
	m_filename(filename)
{
	wxFrame* mainFrame = wxDynamicCast(wxGetTopLevelParent(this), wxFrame);
	statusBar = wxDynamicCast(mainFrame->GetStatusBar(), StatusBar);

	editor = new wxStyledTextCtrl(this, wxID_ANY);
	BindActions();
	editor->SetWrapMode(wxSTC_WRAP_WORD);

	{
		wxString faceName;
		int pointSize;
		bool bold, italic;
		wxConfigBase* config = wxConfig::Get();

		config->Read("Font/FaceName", &faceName, "Consolas");
		config->Read("Font/PointSize", &pointSize, 14);
		config->Read("Font/Bold", &bold, false);
		config->Read("Font/Italic", &italic, false);
		config->Read("Font/Italic", &italic, false);

		wxFont font(pointSize, wxFONTFAMILY_MODERN, (italic) ? wxFONTSTYLE_ITALIC : wxFONTSTYLE_NORMAL, (bold) ? wxFONTWEIGHT_BOLD : wxFONTWEIGHT_NORMAL);
		font.SetFaceName(faceName);
		editor->StyleSetFont(wxSTC_STYLE_DEFAULT, font);
		editor->StyleClearAll();
	}

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(editor, 1, wxEXPAND);
	this->SetSizer(sizer);

	parent->AddPage(this, m_filename, true);

	statusBar->SetEOL(editor->GetEOLMode());
}

TextboxTab::~TextboxTab()
{
	delete file;
}

void TextboxTab::OnSaveFile()
{
	if (m_filepath.empty())
		OnSaveAsFile();
	else
		if (isModified)
			SaveFile();
}

void TextboxTab::OnSaveAsFile()
{
	wxFileDialog saveDialog(this, "Save As", "", "",
		"Text files (*.txt)|*.txt|All files (*.*)|*.*",
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (saveDialog.ShowModal() == wxID_OK) {
		m_filepath = saveDialog.GetPath();
		m_filename = saveDialog.GetFilename();
		SaveFile();
	}
}

void TextboxTab::SetFileContents(const wxString& filepath)
{
	file = new wxFile(filepath);
	if (file->IsOpened()) {
		wxString content;
		file->ReadAll(&content);
		suppressChangeEvent = true;
		editor->SetValue(content);
		suppressChangeEvent = false;
		m_filepath = filepath;
	}
	else {
		wxMessageBox("Failed to open file \"" + filepath + "\".", "File Open Error", wxICON_ERROR);
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

void TextboxTab::OnZoomIn()
{
	editor->ZoomIn();
}

void TextboxTab::OnZoomOut()
{
	editor->ZoomOut();
}

void TextboxTab::OnSetZoom()
{
	editor->SetZoom(0);
}

void TextboxTab::UpdateStatusbar()
{
	int line = editor->GetCurrentLine() + 1;
	int col = editor->GetColumn(editor->GetCurrentPos()) + 1;
	int charCount = editor->GetTextLength();
	statusBar->UpdateInfo(line, col, charCount);
}

void TextboxTab::OnWordWrap(bool wrap)
{
	editor->SetWrapMode(wrap ? wxSTC_WRAP_WORD : wxSTC_WRAP_NONE);
}

void TextboxTab::SaveFile()
{
	isModified = false;
	UpdateTabLabel();
	wxMessageBox("File Saved At: " + m_filepath);
}

void TextboxTab::UpdateTabLabel()
{
	wxAuiNotebook* parentNotebook = wxDynamicCast(GetParent(), wxAuiNotebook);
	if (!parentNotebook) return;

	int pageIndex = parentNotebook->FindPage(this);
	if (pageIndex == wxNOT_FOUND) return;

	if (isModified) {
		parentNotebook->SetPageText(pageIndex, "*" + m_filename);
	}
	else {
		parentNotebook->SetPageText(pageIndex, m_filename);
	}
}

void TextboxTab::BindActions()
{
	editor->Bind(wxEVT_STC_UPDATEUI, [&](wxStyledTextEvent&) {
		UpdateStatusbar();
		});
	editor->Bind(wxEVT_STC_ZOOM, [&](wxStyledTextEvent&) {
		int zoom = editor->GetZoom() * 10;
		statusBar->SetZoom(zoom);
		});
	editor->Bind(wxEVT_STC_CHANGE, [&](wxStyledTextEvent&) {
		if (suppressChangeEvent) return;
		isModified = true;
		UpdateTabLabel();
		});
}
