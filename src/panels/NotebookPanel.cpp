#include "NotebookPanel.h"

NotebookPanel::NotebookPanel(wxWindow* parent)
	: wxPanel(parent, wxID_ANY)
{
	notebook = new wxAuiNotebook(this, wxID_ANY);
	notebook->Bind(wxEVT_AUINOTEBOOK_PAGE_CLOSE, &NotebookPanel::OnNotebookTabClose, this);

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(notebook, 1, wxEXPAND);
	SetSizer(sizer);
	CreateNewTab("Untitled.txt");
}

void NotebookPanel::CreateNewTab(const wxString& filename)
{
	TextboxTab* tab = new TextboxTab(notebook, filename);
	Openedtabs.push_back(tab);
}

void NotebookPanel::SaveCurrentTab()
{
	TextboxTab* tab = GetCurrentTab();
	tab->OnSaveFile();
}

void NotebookPanel::SaveAsCurrentTab()
{
	TextboxTab* tab = GetCurrentTab();
	tab->OnSaveAsFile();
}

void NotebookPanel::SaveAllCurrentTab()
{
}

TextboxTab* NotebookPanel::GetCurrentTab()
{
	wxWindow* page = notebook->GetCurrentPage();
	TextboxTab* tab = dynamic_cast<TextboxTab*>(page);
	return tab;
}

void NotebookPanel::OnNotebookTabClose(wxCommandEvent& e)
{
	TextboxTab* tab = GetCurrentTab();
	if (tab) {
		// Remove from your openTabs vector
		Openedtabs.erase(
			std::remove(Openedtabs.begin(), Openedtabs.end(), tab),
			Openedtabs.end()
		);
	}
	e.Skip();
}
