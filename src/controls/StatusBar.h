#pragma once

#include <wx/wx.h>

class StatusBar : public wxStatusBar {
public:

	StatusBar(wxWindow* parent);

    void SetEOL(int eol);
    void SetZoom(int zoom);
    void UpdateInfo(int line, int col, int charCount);

private:
    enum PaneId {
        POS = 0,
        CHARS,
        ZOOM,
        LINE_ENDINGS,
        ENCODING,
        TOTAL_PANES
    };
    wxString eolMode = "Unknown";
};