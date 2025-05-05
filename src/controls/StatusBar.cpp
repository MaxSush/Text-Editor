#include "StatusBar.h"
#include <wx/stc/stc.h>

StatusBar::StatusBar(wxWindow* parent)
	:
	wxStatusBar(parent,wxID_ANY)
{
    int widths[TOTAL_PANES] = {
            FromDIP(150), // POS
            -1, // CHARS
            FromDIP(100), // ZOOM
            FromDIP(150), // LINE ENDINGS
            FromDIP(100)  // ENCODING
    };
    SetFieldsCount(TOTAL_PANES);
    SetStatusWidths(TOTAL_PANES, widths);

    SetMinHeight(FromDIP(25)); // ensure height for DPI
    Layout();
    SetStatusText(wxString::Format("%d%%", 100), ZOOM);
}

void StatusBar::SetEOL(int eol)
{
    wxString eolMode = "Unknown";
    switch (eol) {
        case wxSTC_EOL_CRLF: 
            eolMode = "Windows (CRLF)"; break;
        case wxSTC_EOL_CR:   
            eolMode = "Mac (CR)"; break;
        case wxSTC_EOL_LF:   
            eolMode = "Unix (LF)"; break;
    }

    this->eolMode = eolMode;
}

void StatusBar::SetZoom(int zoom = 100)
{
    SetStatusText(wxString::Format("%d%%", 100 + zoom), ZOOM);
    Refresh();
}

void StatusBar::UpdateInfo(int line, int col, int charCount)
{
    SetStatusText(wxString::Format("Ln %d, Col %d", line, col), POS);
    SetStatusText(wxString::Format("%d characters", charCount), CHARS);
    SetStatusText(eolMode, LINE_ENDINGS);
    SetStatusText("UTF-8", ENCODING);
    Refresh();
}

