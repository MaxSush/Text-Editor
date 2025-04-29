#include "NotebookPanel.h"

NotebookPanel::NotebookPanel(wxWindow* parent)
	: wxPanel(parent, wxID_ANY)
{
	notebook = new wxAuiNotebook(this, wxID_ANY);
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(notebook, 1, wxEXPAND);
	SetSizer(sizer);

	CreateNewTab("Untitled.txt");
}

void NotebookPanel::CreateNewTab(const std::string& filename)
{
	wxPanel* panel = new wxPanel(notebook, wxID_ANY);

	wxStyledTextCtrl* editor = new wxStyledTextCtrl(panel, wxID_ANY);
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(editor, 1, wxEXPAND);
	panel->SetSizer(sizer);

	notebook->AddPage(panel, filename, true);
	notebook->GetPageCount();
}

int NotebookPanel::GetPageCount() const
{
	return notebook->GetPageCount();
}
