#include "App.h"
#include "MainFrame.h"
#include <wx/wx.h>
#include <wx/icon.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <cstdint>

wxIMPLEMENT_APP(App);

const uint16_t width = 405, height = 700;

bool App::OnInit()
{
    wxString IconPath = "calculator_icon.ico";

    wxIcon appIcon;
    if (!appIcon.LoadFile(IconPath, wxBITMAP_TYPE_ICO)) {
        wxLogError("Failed to load icon from: %s", IconPath);
        return false;
    }

    MainFrame* mainFrame = new MainFrame("Calculator");
    mainFrame->SetClientSize(width, height);
    mainFrame->Center();
    mainFrame->SetIcon(appIcon);
    mainFrame->Show();

    return true;
}