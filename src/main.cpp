#include <wx/wx.h>
#include "frames/MainFrame.h"

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
    virtual int OnExit();
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    std::string name = "Text-Editor";
    wxConfig::Set(new wxConfig(name));
    MainFrame* frame = new MainFrame(name);
    frame->Show(true);
    return true;
}

int MyApp::OnExit()
{
    delete wxConfig::Set(nullptr);
    return 0;
}
