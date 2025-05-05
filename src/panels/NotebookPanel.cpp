#include "NotebookPanel.h"

#include <wx/filename.h>

NotebookPanel::NotebookPanel(wxWindow* parent)
	: wxPanel(parent, wxID_ANY)
{
	notebook = new wxAuiNotebook(this, wxID_ANY);
	notebook->Bind(wxEVT_AUINOTEBOOK_PAGE_CLOSE, [&](wxAuiNotebookEvent& e) {
		RemoveTab();
		e.Skip();
		});
	notebook->Bind(wxEVT_AUINOTEBOOK_PAGE_CHANGED, [&](wxAuiNotebookEvent& event) {
		currentTab = GetCurrentTab();
		currentTab->UpdateStatusbar();
		}
	);

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(notebook, 1, wxEXPAND);
	SetSizer(sizer);
	CreateNewTab("Untitled.txt");
}

void NotebookPanel::CreateNewTab(const wxString& filename)
{
	TextboxTab* tab = new TextboxTab(notebook, filename);
	Openedtabs.push_back(tab);
	currentTab = tab;
}

void NotebookPanel::SaveAllTab()
{
	for (const auto& t : Openedtabs) {
		int index = notebook->GetPageIndex(t);
		notebook->SetSelection(index);
		t->OnSaveFile();
	}
}

void NotebookPanel::CloseCurrentTab()
{
	int index = notebook->GetSelection();
	if (index == wxNOT_FOUND) return;
	notebook->DeletePage(index);
	RemoveTab();
}

void NotebookPanel::OpenFile(const wxString filename, const wxString& filepath)
{
	wxFileName fn1(filepath);
	fn1.Normalize(wxPATH_NORM_ALL);
	for (const auto& tab : Openedtabs) {
		wxFileName fn2(tab->GetFilepath());
		fn2.Normalize(wxPATH_NORM_ALL);
		if (fn1.SameAs(fn2)){
			int index = notebook->GetPageIndex(tab);
			notebook->SetSelection(index);
			return;
		}
	}

	CreateNewTab(filename);
	currentTab->SetFileContents(filepath);
}

TextboxTab* NotebookPanel::GetCurrentTab()
{
	wxWindow* page = notebook->GetCurrentPage();
	return dynamic_cast<TextboxTab*>(page);
}

void NotebookPanel::RemoveTab()
{
	TextboxTab* tab = GetCurrentTab();
	if (tab) {
		// Remove from your openTabs vector
		Openedtabs.erase(
			std::remove(Openedtabs.begin(), Openedtabs.end(), tab),
			Openedtabs.end()
		);
	}
}