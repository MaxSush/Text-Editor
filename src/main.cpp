#include <wx/wx.h>
#include "frames/MainFrame.h"

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    MainFrame* frame = new MainFrame("Text-Editor");
    frame->Show(true);
    return true;
}
