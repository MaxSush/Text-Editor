#include "MainFrame.h"

MainFrame::MainFrame(const std::string& name)
	: wxFrame(NULL, wxID_ANY, name, wxDefaultPosition, { 900,700 }, wxCAPTION | wxCLOSE_BOX | wxMINIMIZE_BOX| wxMAXIMIZE_BOX |wxRESIZE_BORDER | wxNO_BORDER)
{
	//wxIcon icon;
	//icon.LoadFile("path/to/icon.ico", wxBITMAP_TYPE_ICO);  // Load icon from file
	//this->SetIcon(icon);

	CreateStatusBar();
	SetStatusText("Welcome to wxWidgets!");

}
