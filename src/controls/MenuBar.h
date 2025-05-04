#pragma once

#include <wx/wx.h>
#include <wx/filehistory.h>

enum {
    ID_NewTab = wxID_HIGHEST + 1,
    ID_OpenFile,
    ID_Save,
    ID_SaveAs,
    ID_SaveAll,
    ID_CloseTab,
    ID_Exit,
    ID_Undo,
    ID_Redo,
    ID_Cut,
    ID_Copy,
    ID_Paste,
    ID_Delete,

    ID_Find,
    ID_Replace,
    ID_Font,
    ID_SetFont,
    ID_ResetFont,

    ID_StatusBar,
    ID_WordWrap,
    ID_ZoomIn,
    ID_ZoomOut,
    ID_ZoomReset,
    ID_ClearHistory
};

class MenuBar : public wxMenuBar
{
public:
	MenuBar(wxFileHistory* fileHistory);

};
